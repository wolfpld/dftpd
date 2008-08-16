#ifndef __DFTPD__SERVER_HPP__
#define __DFTPD__SERVER_HPP__

#include "ServerPtr.hpp"
#include "Listener.hpp"
#include "SessionController.hpp"

class Server
{
public:
	~Server();

	static ServerPtr Create();

	void Tick();

private:
	Server();

	void InitListener();

	ServerWPtr m_this;

	ListenerPtr m_listener;
	SessionControllerPtr m_sessionController;
};

#endif
