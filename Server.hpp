#ifndef __DFTPD__SERVER_HPP__
#define __DFTPD__SERVER_HPP__

#include "ServerPtr.hpp"
#include "Listener.hpp"
#include "SessionController.hpp"

class Server
{
public:
	static ServerPtr Create();
	~Server();

	void Tick();

	void IncomingConnection( int sock );

private:
	Server();

	void InitListener();

	ServerWPtr m_this;

	ListenerPtr m_listener;
	SessionControllerPtr m_sessionController;
};

#endif
