#ifndef __DFTPD__IO_HPP__
#define __DFTPD__IO_HPP__

#ifdef _WIN32
#include <windows.h>
#define S_ISDIR(mode) (((mode) & _S_IFMT) == (_S_IFDIR))
#define S_ISREG(mode) (((mode) & _S_IFMT) == (_S_IFREG))
#define localtime_r(a,b) localtime_s(b,a)
#else
#include <dirent.h>
#endif

#include <string>

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
#ifdef _WIN32
	HANDLE m_handle;
	WIN32_FIND_DATA m_ffd;
	std::string m_path;
#else
	DIR* m_dir;
	dirent* m_dirent;
#endif
};

#endif
