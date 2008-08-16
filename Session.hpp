#ifndef __DFTPD__SESSION_HPP__
#define __DFTPD__SESSION_HPP__

#include "SessionPtr.hpp"
#include "SessionControllerPtr.hpp"
#include "String.hpp"
#include "Telnet.hpp"
#include "Auth.hpp"

typedef std::vector<std::string> Command;

class Session
{
	enum State
	{
		S_GREETING,
		S_LOGIN,
		S_PASSWORD,
		S_READY
	};

	enum PassState
	{
		PS_NONE,
		PS_LOGGEDIN,
		PS_BADPASS
	};

public:
	static SessionPtr Create( int controlSock, const SessionControllerPtr& sessionController, const AuthPtr& auth );
	~Session();

	void Tick();

private:
	Session( int controlSock, const SessionControllerPtr& sessionController, const AuthPtr& auth );

	void Remove();

	void SendGreeting();
	void SendSyntaxError();
	void SendNotLoggedIn();
	bool AwaitLogin();
	PassState AwaitPassword();
	void AwaitReady();

	Command GetCommand() { return ParseCommand( m_control->GetBuf() ); }


	TelnetPtr m_control;
	int m_controlSock;

	int m_id;
	static int m_counter;

	State m_state;

	SessionControllerWPtr m_sessionController;
	SessionWPtr m_this;
	AuthPtr m_auth;
};

#endif
