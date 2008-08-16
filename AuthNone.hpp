#ifndef __DFTPD__AUTHNONE_HPP__
#define __DFTPD__AUTHNONE_HPP__

#include "Auth.hpp"

class AuthNone : public Auth
{
public:
	bool Login( const std::string& login );
	bool Password( const std::string& password );
};

#endif
