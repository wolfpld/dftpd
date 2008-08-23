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
#include "Server.hpp"
#include "Log.hpp"

Listener::Listener()
	: m_sock( 0 )
{
	// Discover machine's IP address
	char buf[128];
	gethostname( buf, 128 );
	
	hostent* h;
	if( ( h = gethostbyname( buf ) ) == (void*)-1 )
	{
		throw strerror( errno );
	}
	m_ipaddr = inet_ntoa( *((in_addr*)h->h_addr) );
}

Listener::Listener( const std::string& ip )
	: m_sock( 0 )
	, m_ipaddr( ip )
{
}

Listener::~Listener()
{
	if( m_sock != 0 )
	{
		g_log->Print( "[Listener] Closing socket" );
		close( m_sock );
	}
}

void Listener::Listen()
{
	g_log->Print( "[Listener] Opening socket" );

	if( ( m_sock = socket( PF_INET, SOCK_STREAM, 0 ) ) == -1 )
	{
		throw strerror( errno );
	}

	// Reuse port
	int yes = 1;
	if( setsockopt( m_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( int ) ) == -1 )
	{
		throw strerror( errno );
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons( 21 );
	addr.sin_addr.s_addr = inet_addr( m_ipaddr.c_str() );
	memset( addr.sin_zero, 0, sizeof( addr.sin_zero ) );

	if( bind( m_sock, (sockaddr*)&addr, sizeof( addr ) ) == -1 )
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
	sockaddr_in addr;
	socklen_t size = sizeof( addr );

	int incoming = accept( m_sock, (sockaddr*)&addr, &size );
	if( incoming == -1 )
	{
		throw strerror( errno );
	}

	ServerPtr server = m_server.lock();
	if( !server )
	{
		throw "Listener lost server";
	}
	server->IncomingConnection( incoming );
}
