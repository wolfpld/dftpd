#include <iostream>
#include <sys/select.h>
#include <string.h>
#include <errno.h>
#include "Server.hpp"
#include "AuthNone.hpp"

Server::Server()
	: m_listener( new Listener )
	, m_sessionController( new SessionController )
	, m_auth( new AuthNone )
{
	std::cout << "Dumb FTP server\nIP: " << m_listener->GetIPAddr() << std::endl;

	m_listener->Listen();
}

Server::Server( const std::string& ip )
	: m_listener( new Listener( ip ) )
	, m_sessionController( new SessionController )
	, m_auth( new AuthNone )
{
	std::cout << "Dumb FTP server\nIP: " << m_listener->GetIPAddr() << std::endl;

	m_listener->Listen();
}

Server::~Server()
{
	std::cout << "[Server] Shutting down\n";
}

ServerPtr Server::Create()
{
	ServerPtr ret( new Server );
	ret->m_this = ret;

	ret->InitListener();

	return ret;
}

ServerPtr Server::Create( const std::string& ip )
{
	ServerPtr ret( new Server( ip ) );
	ret->m_this = ret;

	ret->InitListener();

	return ret;
}

void Server::Tick()
{
	fd_set read, write;
	std::list<int> fds = m_sessionController->GetFds();
	int maxfd = m_listener->GetSock();

	FD_ZERO( &read );
	FD_ZERO( &write );

	FD_SET( maxfd, &read );

	for( std::list<int>::const_iterator it = fds.begin(); it != fds.end(); ++it )
	{
		maxfd = std::max( maxfd, abs( *it ) );

		if( *it < 0 )
		{
			// Hack
			FD_SET( -(*it), &write );
		}
		else
		{
			FD_SET( *it, &read );
		}
	}

	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 1000000;

	int ret = select( maxfd + 1, &read, &write, NULL, &tv );

	if( ret == -1 )
	{
		throw strerror( errno );
	}
	else if( ret == 0 )
	{
		// Descriptors not ready
		return;
	}

	std::list<int> activeFds;
	for( std::list<int>::const_iterator it = fds.begin(); it != fds.end(); ++it )
	{
		if( ( *it > 0 && FD_ISSET( *it, &read ) ) ||
		    ( *it < 0 && FD_ISSET( -(*it), &write ) ) )
		{
			activeFds.push_back( abs( *it ) );
		}
	}

	if( activeFds.size() != 0 )
	{
		m_sessionController->Tick( activeFds );
	}

	if( FD_ISSET( m_listener->GetSock(), &read ) )
	{
		m_listener->Tick();
	}
}

void Server::IncomingConnection( int sock )
{
	m_sessionController->Add( Session::Create( sock, m_sessionController, m_auth, m_listener->GetIPAddr() ) );
}

void Server::InitListener()
{
	m_listener->SetServer( m_this );
}
