#include <ctype.h>
#include <algorithm>
#include "String.hpp"
#include "Exceptions.hpp"

inline static bool IsSpace( char c )
{
	return isspace( c );
}

inline static bool IsNotSpace( char c )
{
	return !isspace( c );
}

inline static bool IsSlash( char c )
{
	return c == '/';
}

inline static bool IsNotSlash( char c )
{
	return c != '/';
}

inline static bool IsComma( char c )
{
	return c == ',';
}

inline static bool IsNotComma( char c )
{
	return c != ',';
}

PathVector SplitPath( const std::string& str )
{
	PathVector ret;

	typedef std::string::const_iterator Iter;

	Iter i = str.begin(), j;

	while( i != str.end() )
	{
		i = std::find_if( i, str.end(), IsNotSlash );
		j = std::find_if( i, str.end(), IsSlash );

		if( i != str.end() )
		{
			ret.push_back( std::string( i, j ) );
		}

		i = j;
	}

	if( ret.size() > 0 && ret[0] == "~" )
	{
		ret.erase( ret.begin() );
	}

	return ret;
}

PortVector SplitPort( const std::string& str )
{
	PortVector ret;

	typedef std::string::const_iterator Iter;

	Iter i = str.begin(), j;

	while( i != str.end() )
	{
		i = std::find_if( i, str.end(), IsNotComma );
		j = std::find_if( i, str.end(), IsComma );

		if( i != str.end() )
		{
			ret.push_back( std::string( i, j ) );
		}

		i = j;
	}

	return ret;
}

Command Split( const std::string& str )
{
	Command ret;

	typedef std::string::const_iterator Iter;

	Iter i = str.begin(), j;

	while( i != str.end() )
	{
		i = std::find_if( i, str.end(), IsNotSpace );
		j = std::find_if( i, str.end(), IsSpace );

		if( i != str.end() )
		{
			ret.push_back( std::string( i, j ) );
		}

		i = j;
	}

	return ret;
}

Command ParseCommand( const std::string& cmd )
{
	Command ret( Split( cmd ) );

	if( cmd.size() == 0 )
	{
		throw SyntaxErrorException;
	}

	ToUpper( ret[0] );

	return ret;
}

void ToUpper( std::string& str )
{
	std::transform( str.begin(), str.end(), str.begin(), toupper );
}
