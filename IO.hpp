#ifndef __DFTPD__IO_HPP__
#define __DFTPD__IO_HPP__

#include <string>

class IO
{
public:
	static bool MkDir( const std::string& dir );
	static bool RmDir( const std::string& dir );

private:
	IO() {};
};

#endif
