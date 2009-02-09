#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <boost/lexical_cast.hpp>
#include "Filesystem.hpp"
#include "IO.hpp"
#include "String.hpp"

Filesystem::Filesystem( const std::string& root )
	: m_root( root )
	, m_path( "/" )
{
#ifdef SYMBIAN
	m_rfs.Connect();
#endif
}

Filesystem::~Filesystem()
{
#ifdef SYMBIAN
	m_rfs.Close();
#endif
}

bool Filesystem::ChangeDirectory( const std::string& cd )
{
	PathVector reqPath = SplitPath( cd );
	PathVector path = SplitProperPath( cd );

	if( !TryChangePath( reqPath, path ) )
	{
		return false;
	}

	m_path = MakePath( path );

	return true;
}

bool Filesystem::FileExists( const std::string& file )
{
	return CheckFileExists( GetFilePath( file ) );
}

FILE* Filesystem::FileOpen( const std::string& file, Mode mode )
{
	std::string path = m_root + GetFilePath( file );

	switch( mode )
	{
	case M_READ:
		return fopen( path.c_str(), "rb" );
		break;

	case M_WRITE:
		return fopen( path.c_str(), "wb" );
		break;

	default:
		break;
	}

	return NULL;
}

#ifdef SYMBIAN
RFile* Filesystem::FileOpenSymbian( const std::string& file, Mode mode )
{
	RFile* f = new RFile;

	std::string path = m_root + GetFilePath( file );

	for( unsigned int i=0; i<path.size(); i++ )
	{
		if( path[i] == '/' || path[i] == '\\' )
		{
			if( i>0 && path[i-1] == '\\' )
			{
				path.erase( i, 1 );
				i--;
			}
			else
			{
				path[i] = '\\';
			}
		}
	}

	TPtrC8 ptr( reinterpret_cast<const TUint8*>( path.c_str() ) );
	TBuf<512> buf;
	buf.FillZ();
	buf.Copy( ptr );

	switch( mode )
	{
	case M_READ:
		if( f->Open( m_rfs, buf, EFileRead | EFileStream | EFileShareReadersOnly ) != KErrNone )
		{
			delete f;
			f = NULL;
		}
		break;

	case M_WRITE:
		if( f->Replace( m_rfs, buf, EFileWrite | EFileStream ) != KErrNone )
		{
			delete f;
			f = NULL;
		}
		break;

	default:
		delete f;
		f = NULL;
		break;
	}

	return f;
}
#endif

std::list<std::string> Filesystem::GetListing( const std::string& path )
{
	std::list<std::string> ret;

	time_t now = time( NULL );

	std::string filePath = GetFilePath( path );
	if( CheckFileExists( filePath ) )
	{
		struct stat s;
		stat( ( m_root + filePath ).c_str(), &s );

		std::string entry = "-rw-r--r--";

		tm timeFile;
		localtime_r( &s.st_mtime, &timeFile );

		char dateBuf[64];
		if( now > s.st_mtime && now - s.st_mtime < 60*60*24*180 )
		{
			// File is "recent"
			strftime( dateBuf, sizeof( dateBuf ), " %b %e %H:%M ", &timeFile );
		}
		else
		{
			strftime( dateBuf, sizeof( dateBuf ), " %b %e  %Y ", &timeFile );
		}

		entry += " 1 root root " + boost::lexical_cast<std::string>( s.st_size ) + dateBuf + path;

		ret.push_back( entry );

		return ret;
	}

	PathVector reqPath = SplitPath( path );
	PathVector pv = SplitProperPath( path );

	if( !TryChangePath( reqPath, pv ) )
	{
		return ret;
	}

	std::string newPath = m_root + MakePath( pv );

	Directory d;
	if( !d.Open( newPath ) )
	{
		return ret;
	}

	tm timeNow;
	localtime_r( &now, &timeNow );

	// Symbian readdir() doesn't report "." and ".."
	bool hasDot = false;
	bool hasDotDot = false;

	struct stat s;
	while( d )
	{
		std::string dirName = d.GetName();
		stat( ( newPath + "/" + dirName ).c_str(), &s );

		std::string entry;

		if( S_ISDIR( s.st_mode ) )
		{
			entry += "drwxr-xr-x";
		}
		else
		{
			entry += "-rw-r--r--";
		}

		tm timeFile;
		localtime_r( &s.st_mtime, &timeFile );

		char dateBuf[64];
		if( now > s.st_mtime && now - s.st_mtime < 60*60*24*180 )
		{
			// File is "recent"
			strftime( dateBuf, sizeof( dateBuf ), " %b %e %H:%M ", &timeFile );
		}
		else
		{
			strftime( dateBuf, sizeof( dateBuf ), " %b %e  %Y ", &timeFile );
		}

		entry += " 1 root root " + boost::lexical_cast<std::string>( s.st_size ) + dateBuf + dirName;

		if( dirName == "." )
		{
			hasDot = true;
		}
		else if( dirName == ".." )
		{
			hasDotDot = true;
		}

		ret.push_back( entry );

		++d;
	}

	if( !hasDotDot )
	{
		ret.push_front( "drwxr-xr-x 1 root root 0 Jan  1  1970 .." );
	}
	if( !hasDot )
	{
		ret.push_front( "drwxr-xr-x 1 root root 0 Jan  1  1970 ." );
	}

	return ret;
}

bool Filesystem::Delete( const std::string& file )
{
	std::string path = m_root + GetFilePath( file );

	return unlink( path.c_str() ) == 0;
}

std::string Filesystem::MkDir( const std::string& dir )
{
	std::string filePath = GetFilePath( dir );
	if( filePath == "" )
	{
		return "";
	}

	std::string path = m_root + filePath;

	if( !IO::MkDir( path ) )
	{
		return "";
	}

	return path;
}

bool Filesystem::RmDir( const std::string& dir )
{
	std::string filePath = GetFilePath( dir );
	if( filePath == "" )
	{
		return false;
	}

	std::string path = m_root + filePath;

	if( !IO::RmDir( path ) )
	{
		return false;
	}

	return true;
}

std::string Filesystem::MakePath( const PathVector& pv )
{
	std::string ret;

	if( pv.size() == 0 )
	{
		ret = "/";
	}
	else
	{
		for( PathVector::const_iterator it = pv.begin(); it != pv.end(); ++it )
		{
			ret.append( "/" );
			ret.append( *it );
		}
	}

	return ret;
}

bool Filesystem::DirectoryExists( const std::string& dir )
{
	std::string path = m_root + dir;
	struct stat s;

	if( stat( path.c_str(), &s ) == -1 )
	{
		return false;
	}

	return S_ISDIR( s.st_mode );
}

bool Filesystem::TryChangePath( const PathVector& reqPath, PathVector& path )
{
	for( PathVector::const_iterator it = reqPath.begin(); it != reqPath.end(); ++it )
	{
		if( *it == "." )
		{
			continue;
		}
		else if( *it == ".." )
		{
			if( path.size() == 0 )
			{
				// Can't go below the root directory.
				// Or maybe it should be possible as no-op?
				return false;
			}
			else
			{
				path.pop_back();
			}
		}
		else
		{
			if( DirectoryExists( MakePath( path ) + "/" + *it ) )
			{
				path.push_back( *it );
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}

bool Filesystem::CheckFileExists( const std::string& file )
{
	std::string path = m_root + file;
	struct stat s;

	if( stat( path.c_str(), &s ) == -1 )
	{
		return false;
	}

	return S_ISREG( s.st_mode );
}

std::string Filesystem::GetFilePath( const std::string& file )
{
	PathVector reqPath = SplitPath( file );

	if( reqPath.size() == 0 )
	{
		return "";
	}

	// Remove filename from path
	std::string fname = reqPath.back();
	reqPath.pop_back();

	PathVector path = SplitProperPath( file );

	if( !TryChangePath( reqPath, path ) )
	{
		return "";
	}

	return MakePath( path ) + "/" + fname;
}

PathVector Filesystem::SplitProperPath( const std::string& path )
{
	PathVector ret;

	if( path.length() > 0 &&
	    ( path[0] == '/' ||
	    ( path.length() > 1 && ( path[0] == '~' && path[1] == '/' ) ) ) )
	{
		ret = SplitPath( "/" );
	}
	else
	{
		ret = SplitPath( m_path );
	}

	return ret;
}
