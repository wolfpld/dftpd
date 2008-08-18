#include <iostream>
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

void Server::Tick()
{
	m_listener->Tick();
	m_sessionController->Tick();
}

void Server::IncomingConnection( int sock )
{
	m_sessionController->Add( Session::Create( sock, m_sessionController, m_auth, m_listener->GetIPAddr() ) );
}

void Server::InitListener()
{
	m_listener->SetServer( m_this );
}
