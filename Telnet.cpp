#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>
#include "Telnet.hpp"
#include "Exceptions.hpp"

static const char CRLF[] = { 13, 10, 0 };
static const char IAC[] = { 255, 0 };

Telnet::Telnet( int sock )
	: m_sock( sock )
{
}

Telnet::~Telnet()
{
}

TelnetPtr Telnet::Create( int sock )
{
	TelnetPtr ret( new Telnet( sock ) );
	ret->m_this = ret;
	ret->m_cmd.reset( new TelnetCommand( ret ) );

	return ret;
}

bool Telnet::Read()
{
	std::string buf;

	// Read all that's waiting on the socket
	for(;;)
	{
		char tmpBuf[BufSize];

		fd_set fd;
		FD_ZERO( &fd );
		FD_SET( m_sock, &fd );
		timeval tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		select( m_sock + 1, &fd, NULL, NULL, &tv );
		if( !FD_ISSET( m_sock, &fd ) )
		{
			break;
		}

		int size = recv( m_sock, tmpBuf, BufSize, 0 );

		if( size == -1 )
		{
			if( errno == EAGAIN )
			{
				// No error, just nothing to be read from socket
				break;
			}

			throw SessionErrorException;
		}
		else if( size == 0 )
		{
			throw ConnectionTerminatedException;
		}

		buf.append( tmpBuf, size );
	}

	// Parse telnet commands
	for( unsigned int i=0; i<buf.size(); i++ )
	{
		if( m_cmd->ParsingCommand() || (unsigned char)buf[i] == 255 )
		{
			if( !m_cmd->Parse( buf[i] ) )
			{
				throw SessionErrorException;
			}
		}
		else
		{
			m_readBuf += buf[i];
		}
	}

	return m_readBuf.find( CRLF ) != std::string::npos;
}

void Telnet::Write( const std::string& msg )
{
	std::string buf;
	for( unsigned int i=0; i<msg.size(); i++ )
	{
		buf += msg[i];

		if( (unsigned char)msg[i] == 255 )
		{
			buf.append( IAC );
		}
	}
	buf.append( CRLF );

	unsigned int pos = 0;
	char *ptr = (char*)buf.c_str();

	while( pos != buf.size() )
	{
		int size = send( m_sock, ptr, buf.size() - pos, 0 );

		if( size == -1 )
		{
			throw SessionErrorException;
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
		throw SessionErrorException;
	}

	ret = m_readBuf.substr( 0, pos );

	m_readBuf.erase( 0, pos + 2 );

	return ret;
}

void Telnet::EraseCharacter()
{
	if( m_readBuf.size() > 0 )
	{
		m_readBuf.erase( m_readBuf.end() - 1 );
	}
}

void Telnet::EraseLine()
{
	throw SessionErrorException;
}
