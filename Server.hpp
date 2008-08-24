#ifndef __DFTPD__SERVER_HPP__
#define __DFTPD__SERVER_HPP__

#include <list>
#include <string>
#include "ServerPtr.hpp"
#include "Listener.hpp"
#include "SessionController.hpp"
#include "Auth.hpp"

class Server
{
public:
	static ServerPtr Create( const AuthPtr& auth );
	static ServerPtr Create( const AuthPtr& auth, const std::string& ip );
	~Server();

	void Tick();

	void IncomingConnection( int sock );

	const std::list<std::string>& GetWelcomeMessage() const { return m_welcome; }

private:
	Server( const AuthPtr& auth );
	Server( const AuthPtr& auth, const std::string& ip );

	void InitListener();
	void LoadWelcomeMessage();

	ServerWPtr m_this;

	ListenerPtr m_listener;
	SessionControllerPtr m_sessionController;
	AuthPtr m_auth;

	std::list<std::string> m_welcome;
};

#endif
