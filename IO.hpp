#ifndef __DFTPD__IO_HPP__
#define __DFTPD__IO_HPP__

#include <string>
#include <dirent.h>

class IO
{
public:
	static bool MkDir( const std::string& dir );
	static bool RmDir( const std::string& dir );

private:
	IO() {};
};

class Directory
{
public:
	Directory();
	~Directory();

	bool Open( const std::string& dir );

	Directory& operator++();
	operator bool();

	std::string GetName();

private:
	DIR* m_dir;
	dirent* m_dirent;
};

#endif
