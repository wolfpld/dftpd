#include <sys/stat.h>
#include "Filesystem.hpp"
#include "String.hpp"

Filesystem::Filesystem( const std::string& root )
	: m_root( root )
	, m_path( "/" )
{
}

Filesystem::~Filesystem()
{
}

bool Filesystem::ChangeDirectory( const std::string& cd )
{
	PathVector reqPath = SplitPath( cd );
	PathVector path;
	if( cd[0] == '/' )
	{
		path = SplitPath( "/" );
	}
	else
	{
		path = SplitPath( m_path );
	}

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

	// Remove filename from path
	std::string fname = reqPath.back();
	reqPath.pop_back();

	PathVector path;
	if( file[0] == '/' )
	{
		path = SplitPath( "/" );
	}
	else
	{
		path = SplitPath( m_path );
	}

	if( !TryChangePath( reqPath, path ) )
	{
		return false;
	}

	return MakePath( path ) + "/" + fname;
}
