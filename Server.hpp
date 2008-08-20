#ifndef __DFTPD__SERVER_HPP__
#define __DFTPD__SERVER_HPP__

#include <string>
#include "ServerPtr.hpp"
#include "Listener.hpp"
#include "SessionController.hpp"
#include "Auth.hpp"

class Server
{
public:
	static ServerPtr Create();
	static ServerPtr Create( const std::string& ip );
	~Server();

	void Tick();

	void IncomingConnection( int sock );

private:
	Server();
	Server( const std::string& ip );

	void InitListener();

	ServerWPtr m_this;

	ListenerPtr m_listener;
	SessionControllerPtr m_sessionController;
	AuthPtr m_auth;
};

#endif
