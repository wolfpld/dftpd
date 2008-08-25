#ifndef __DFTPD__FILESYSTEM_HPP__
#define __DFTPD__FILESYSTEM_HPP__

#include <stdio.h>
#include <string>
#include <list>
#include <boost/shared_ptr.hpp>
#include "String.hpp"

#ifdef SYMBIAN
#include <f32file.h>
#endif

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

#ifdef SYMBIAN
	RFile* FileOpenSymbian( const std::string& file, Mode mode );
#endif

	std::list<std::string> GetListing( const std::string& path );

	bool Delete( const std::string& file );

private:
	std::string MakePath( const PathVector& pv );
	bool DirectoryExists( const std::string& dir );
	bool TryChangePath( const PathVector& reqPath, PathVector& path );
	bool CheckFileExists( const std::string& file );
	std::string GetFilePath( const std::string& path );
	PathVector SplitProperPath( const std::string& path );

	std::string m_root;
	std::string m_path;

#ifdef SYMBIAN
	RFs m_rfs;
#endif
};

typedef boost::shared_ptr<Filesystem> FilesystemPtr;

#endif
