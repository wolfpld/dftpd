#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include "IO.hpp"

bool IO::MkDir( const std::string& dir )
{
	return mkdir( dir.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH ) == 0;
}

bool IO::RmDir( const std::string& dir )
{
	return rmdir( dir.c_str() ) == 0;
}

Directory::Directory()
	: m_dir( NULL )
{
}

Directory::~Directory()
{
	closedir( m_dir );
}

bool Directory::Open( const std::string& dir )
{
	m_dir = opendir( dir.c_str() );
	m_dirent = readdir( m_dir );

	return m_dir != NULL;
}

std::string Directory::GetName()
{
	return m_dirent->d_name;
}

Directory& Directory::operator++()
{
	m_dirent = readdir( m_dir );
	return *this;
}

Directory::operator bool()
{
	return m_dirent != NULL;
}
