#ifndef __DFTPD__AUTHTOKEN_HPP__
#define __DFTPD__AUTHTOKEN_HPP__

#include "Auth.hpp"

class AuthToken : public Auth
{
public:
	AuthToken();

	bool Login( const std::string& login );
	bool Password( const std::string& login, const std::string& password );
	std::string GetRoot( const std::string& login );

	const std::string& GetToken() const { return m_token; }

	void GenerateToken();

private:
	std::string m_token;

	bool TokenExists();
};

#endif
