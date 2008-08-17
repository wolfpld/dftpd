#ifndef __DFTPD__SESSION_HPP__
#define __DFTPD__SESSION_HPP__

#include "Auth.hpp"
#include "Data.hpp"
#include "Filesystem.hpp"
#include "SessionControllerPtr.hpp"
#include "SessionPtr.hpp"
#include "String.hpp"
#include "Telnet.hpp"

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

	void HandleMode( const Command& cmd );
	void HandleType( const Command& cmd );
	void HandleStru( const Command& cmd );

	void PrintDirectory();
	void ChangeDirectory( const Command& cmd );
	void ChangeDirectory( const std::string& cmd );

	Command GetCommand() { return ParseCommand( m_control->GetBuf() ); }


	TelnetPtr m_control;
	int m_controlSock;

	int m_dataPort;
	std::string m_dataAddress;
	DataPtr m_data;

	int m_id;
	static int m_counter;

	State m_state;

	SessionControllerWPtr m_sessionController;
	SessionWPtr m_this;
	AuthPtr m_auth;
	FilesystemPtr m_filesystem;

	std::string m_user;
};

#endif
