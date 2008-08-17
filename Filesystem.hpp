#ifndef __DFTPD__FILESYSTEM_HPP__
#define __DFTPD__FILESYSTEM_HPP__

#include <string>
#include <boost/shared_ptr.hpp>
#include "String.hpp"

class Filesystem
{
public:
	Filesystem( const std::string& root );
	~Filesystem();

	const std::string& GetPath() const { return m_path; }
	bool ChangeDirectory( const std::string& cd );

private:
	std::string MakePath( const PathVector& pv );
	bool DirectoryExists( const std::string& dir );

	std::string m_root;
	std::string m_path;
};

typedef boost::shared_ptr<Filesystem> FilesystemPtr;

#endif
