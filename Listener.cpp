#include <unistd.h>
#include <netdb.h>
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
		close( m_sock );
	}
}
