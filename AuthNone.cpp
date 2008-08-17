#include "AuthNone.hpp"

bool AuthNone::Login( const std::string& login )
{
	return true;
}

bool AuthNone::Password( const std::string& login, const std::string& password )
{
	return true;
}

std::string AuthNone::GetRoot( const std::string& login )
{
	return ".";
}
