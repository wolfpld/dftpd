#ifdef _WIN32
#include <winsock.h>
typedef int socklen_t;
#else
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "Listener.hpp"
#include "Server.hpp"
#include "Log.hpp"
#include "Exceptions.hpp"

Listener::Listener()
	: m_sock( 0 )
{
#ifdef _WIN32
	WSADATA wsaData;
	WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
#endif

	// Discover machine's IP address
	char buf[128];
	gethostname( buf, 128 );

	hostent* h;
	if( ( h = gethostbyname( buf ) ) == (void*)-1 )
	{
		g_log->Print( strerror( errno ) );
		throw ServerCrashException;
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
#ifdef _WIN32
		closesocket( m_sock );
#else
		close( m_sock );
#endif
	}
}

void Listener::Listen()
{
	g_log->Print( "[Listener] Opening socket" );

	if( ( m_sock = socket( PF_INET, SOCK_STREAM, 0 ) ) == -1 )
	{
		g_log->Print( strerror( errno ) );
		throw ServerCrashException;
	}

	// Reuse port
#ifdef _WIN32
	char yes = 1;
#else
	int yes = 1;
#endif
	if( setsockopt( m_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof( int ) ) == -1 )
	{
		g_log->Print( strerror( errno ) );
		throw ServerCrashException;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons( 21 );
	addr.sin_addr.s_addr = inet_addr( m_ipaddr.c_str() );
	memset( addr.sin_zero, 0, sizeof( addr.sin_zero ) );

	if( bind( m_sock, (sockaddr*)&addr, sizeof( addr ) ) == -1 )
	{
		g_log->Print( strerror( errno ) );
		g_log->Print( "[Listener] Unable to bind to port 21, retrying with port 2121" );
		
		addr.sin_port = htons( 2121 );

		if( bind( m_sock, (sockaddr*)&addr, sizeof( addr ) ) == -1 )
		{
			g_log->Print( strerror( errno ) );
			throw ServerCrashException;
		}
	}

	if( listen( m_sock, 5 ) == -1 )
	{
		g_log->Print( strerror( errno ) );
		throw ServerCrashException;
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
		g_log->Print( strerror( errno ) );
		throw ServerCrashException;
	}

	ServerPtr server = m_server.lock();
	server->IncomingConnection( incoming );
}
