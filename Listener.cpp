#include <iostream>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Listener.hpp"

Listener::Listener()
	: m_sock( 0 )
{
	// Discover machine's IP address
	char buf[128];
	gethostname( buf, 128 );
	
	struct hostent* h;
	if( ( h = gethostbyname( buf ) ) == (void*)-1 )
	{
		throw strerror( errno );
	}
	m_ipaddr = inet_ntoa( *((struct in_addr*)h->h_addr) );
}

Listener::~Listener()
{
	if( m_sock != 0 )
	{
		std::cout << "[Listener] Closing socket" << std::endl;
		if( close( m_sock ) == -1 )
		{
			throw strerror( errno );
		}
	}
}

void Listener::Listen()
{
	std::cout << "[Listener] Opening socket" << std::endl;

	if( ( m_sock = socket( PF_INET, SOCK_STREAM, 0 ) ) == -1 )
	{
		throw strerror( errno );
	}

	// Set socket to non-blocking
	if( fcntl( m_sock, F_SETFL, O_NONBLOCK ) == -1 )
	{
		throw strerror( errno );
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons( 21 );
	addr.sin_addr.s_addr = inet_addr( m_ipaddr.c_str() );
	memset( addr.sin_zero, 0, sizeof( addr.sin_zero ) );

	if( bind( m_sock, (struct sockaddr*)&addr, sizeof( addr ) ) == -1 )
	{
		throw strerror( errno );
	}

	if( listen( m_sock, 5 ) == -1 )
	{
		throw strerror( errno );
	}
}

void Listener::Tick()
{
	// Check if there are some incoming connections
	struct sockaddr_in addr;
	socklen_t size = sizeof( addr );

	int incoming = accept( m_sock, (struct sockaddr*)&addr, &size );
	if( incoming == -1 )
	{
		if( errno == EAGAIN )
		{
			// No error, just no connections in queue
			return;
		}
		throw strerror( errno );
	}

	std::cout << "Incoming connection\n";
}
