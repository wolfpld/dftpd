#include <stdlib.h>
#include <time.h>
#include "AuthToken.hpp"
#include "Log.hpp"

AuthToken::AuthToken()
	: m_token( "" )
{
	srand( time( NULL ) );
}

bool AuthToken::Login( const std::string& login )
{
	return true;
}

bool AuthToken::Password( const std::string& login, const std::string& password )
{
	if( !TokenExists() )
	{
		return false;
	}

	if( m_token == password )
	{
		// Token is one time use only
		m_token = "";
		return true;
	}
	else
	{
		return false;
	}
}

std::string AuthToken::GetRoot( const std::string& login )
{
	return "/";
}

void AuthToken::GenerateToken()
{
	int tokenLen = 6;

	m_token.clear();

	for( int i=0; i<tokenLen; i++ )
	{
		char c = 48 + ( rand() % ( 125 - 48 ) );
		m_token += c;
	}

	g_log->Print( std::string( "New token: " ) + m_token );
}

bool AuthToken::TokenExists()
{
	return m_token != "";
}
