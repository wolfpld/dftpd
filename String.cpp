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

std::vector<std::string> Split( const std::string& str )
{
	std::vector<std::string> ret;

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

std::vector<std::string> ParseCommand( const std::string& cmd )
{
	std::vector<std::string> ret( Split( cmd ) );

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
