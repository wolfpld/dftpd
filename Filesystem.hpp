#ifndef __DFTPD__FILESYSTEM_HPP__
#define __DFTPD__FILESYSTEM_HPP__

#include <string>
#include <boost/shared_ptr.hpp>

class Filesystem
{
public:
	Filesystem( const std::string& root );
	~Filesystem();

private:
	std::string m_root;
};

typedef boost::shared_ptr<Filesystem> FilesystemPtr;

#endif
