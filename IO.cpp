#include <sys/stat.h>
#include <unistd.h>
#include "IO.hpp"

bool IO::MkDir( const std::string& dir )
{
	return mkdir( dir.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH ) == 0;
}

bool IO::RmDir( const std::string& dir )
{
	return rmdir( dir.c_str() ) == 0;
}
