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

	m_path = MakePath( path );

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

	return (s.st_mode & S_IFMT) == S_IFDIR;
}
