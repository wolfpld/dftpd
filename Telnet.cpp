#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include "Telnet.hpp"

ConnectionTerminated ConnectionTerminatedException;

Telnet::Telnet( int sock )
	: m_sock( sock )
{
}

Telnet::~Telnet()
{
}

bool Telnet::Read()
{
	char buf[1024];

	int size = recv( m_sock, buf, 1024, 0 );
	if( size == -1 )
	{
		throw strerror( errno );
	}
	else if( size == 0 )
	{
		throw ConnectionTerminatedException;
	}
}

std::string Telnet::GetBuf()
{
}
