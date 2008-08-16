#include <iostream>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "Server.hpp"

Server::Server()
	: m_listener( new Listener )
	, m_sessionController( new SessionController )
{
	std::cout << "Dumb FTP server\nIP: " << m_listener->GetIPAddr() << std::endl;

	m_listener->Listen();
}

Server::~Server()
{
	std::cout << "Shutting down server\n";
}

ServerPtr Server::Create()
{
	ServerPtr ret( new Server );
	ret->m_this = ret;

	ret->InitListener();

	return ret;
}

void Server::Tick()
{
	m_listener->Tick();
	m_sessionController->Tick();
}

void Server::IncomingConnection( int sock )
{
	sockaddr_in addr;
	socklen_t size = sizeof( sockaddr_in );

	if( getpeername( sock, (sockaddr*)&addr, &size ) == -1 )
	{
		throw strerror( errno );
	}

	std::cout << "Incoming connection from " << inet_ntoa( addr.sin_addr ) << std::endl;

	m_sessionController->Add( Session::Create( sock, m_sessionController ) );
}

void Server::InitListener()
{
	m_listener->SetServer( m_this );
}