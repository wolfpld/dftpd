#ifndef __DFTPD__AUTH_HPP__
#define __DFTPD__AUTH_HPP__

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class Auth
{
public:
	virtual ~Auth() {}

	virtual bool Login( const std::string& login ) = 0;
	virtual bool Password( const std::string& password ) = 0;
	virtual std::string GetRoot( const std::string& login ) = 0;
};

typedef boost::shared_ptr<Auth> AuthPtr;
typedef boost::weak_ptr<Auth> AuthWPtr;

#endif
