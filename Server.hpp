#ifndef __DFTPD__SERVER_HPP__
#define __DFTPD__SERVER_HPP__

#include "Listener.hpp"
#include "SessionController.hpp"

class Server
{
public:
	Server();
	~Server();

	void Tick();

private:
	ListenerPtr m_listener;
	SessionControllerPtr m_sessionController;
};

#endif
