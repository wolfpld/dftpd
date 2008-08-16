#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include "Telnet.hpp"
#include "Exceptions.hpp"

const char CRLF[] = { 13, 10, 0 };

Telnet::Telnet( int sock )
	: m_sock( sock )
{
}

Telnet::~Telnet()
{
}

bool Telnet::Read()
{
	std::string buf;

	// Read all that's waiting on the socket
	for(;;)
	{
		char tmpBuf[1024];
		int size = recv( m_sock, tmpBuf, 1024, 0 );

		if( size == -1 )
		{
			if( errno == EAGAIN )
			{
				// No error, just nothing to be read from socket
				break;
			}

			throw strerror( errno );
		}
		else if( size == 0 )
		{
			throw ConnectionTerminatedException;
		}

		buf.append( tmpBuf, size );
	}

	// Parse according to telnet protocol
	/*
	for( unsigned int i=0; i<buf.size(); i++ )
	{

	}
	*/
	// temporary
	m_readBuf.append( buf );

	return m_readBuf.find( CRLF ) != std::string::npos;
}

void Telnet::Write( const std::string& msg )
{
	std::string buf( msg );
	buf.append( CRLF );

	unsigned int pos = 0;
	char *ptr = (char*)buf.c_str();

	while( pos != buf.size() )
	{
		int size = send( m_sock, ptr, buf.size() - pos, 0 );

		if( size == -1 )
		{
			throw strerror( errno );
		}
		else if( size == 0 )
		{
			throw ConnectionTerminatedException;
		}

		pos += size;
		ptr += size;
	}
}

std::string Telnet::GetBuf()
{
	std::string ret;

	unsigned int pos = m_readBuf.find( CRLF );
	if( pos == std::string::npos )
	{
		throw "Trying to get telnet buffer when buffer not ready";
	}

	ret = m_readBuf.substr( 0, pos );

	m_readBuf.erase( 0, pos + 1 );

	return ret;
}
