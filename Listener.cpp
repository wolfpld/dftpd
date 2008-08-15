#include <iostream>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
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
	
	struct hostent* h = gethostbyname( buf );
	m_ipaddr = inet_ntoa( *((struct in_addr*)h->h_addr) );
}

Listener::~Listener()
{
	if( m_sock != 0 )
	{
		std::cout << "[Listener] Closing socket" << std::endl;
		close( m_sock );
	}
}

void Listener::Listen()
{
	std::cout << "[Listener] Opening socket" << std::endl;

	m_sock = socket( PF_INET, SOCK_STREAM, 0 );

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons( 21 );
	addr.sin_addr.s_addr = inet_addr( m_ipaddr.c_str() );
	memset( addr.sin_zero, 0, sizeof( addr.sin_zero ) );

	bind( m_sock, (struct sockaddr*)&addr, sizeof( addr ) );
}
