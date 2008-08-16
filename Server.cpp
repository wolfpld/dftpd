#include <iostream>
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
}

void Server::Tick()
{
	m_listener->Tick();
	m_sessionController->Tick();
}
