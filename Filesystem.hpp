#ifndef __DFTPD__FILESYSTEM_HPP__
#define __DFTPD__FILESYSTEM_HPP__

#include <stdio.h>
#include <string>
#include <boost/shared_ptr.hpp>
#include "String.hpp"

class Filesystem
{
public:
	enum Mode
	{
		M_READ,
		M_WRITE
	};

	Filesystem( const std::string& root );
	~Filesystem();

	const std::string& GetPath() const { return m_path; }
	bool ChangeDirectory( const std::string& cd );

	bool FileExists( const std::string& file );
	FILE* FileOpen( const std::string& file, Mode mode );

private:
	std::string MakePath( const PathVector& pv );
	bool DirectoryExists( const std::string& dir );
	bool TryChangePath( const PathVector& reqPath, PathVector& path );
	bool CheckFileExists( const std::string& file );
	std::string GetFilePath( const std::string& path );

	std::string m_root;
	std::string m_path;
};

typedef boost::shared_ptr<Filesystem> FilesystemPtr;

#endif