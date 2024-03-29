#ifndef __DFTPD__SESSION_HPP__
#define __DFTPD__SESSION_HPP__

#include <list>
#include <string>
#include "Auth.hpp"
#include "Data.hpp"
#include "Filesystem.hpp"
#include "ServerPtr.hpp"
#include "SessionControllerPtr.hpp"
#include "SessionPtr.hpp"
#include "String.hpp"
#include "Telnet.hpp"
#include "TelnetPtr.hpp"

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
	static SessionPtr Create( int controlSock, const SessionControllerPtr& sessionController, const AuthPtr& auth, const std::string& ip, const ServerWPtr& server );
	~Session();

	void Tick();

	void DataConnectionFinished();
	void DataConnectionError();
	void OutOfSpace();

	std::list<int> GetFds() const;

private:
	Session( int controlSock, const SessionControllerPtr& sessionController, const AuthPtr& auth, const std::string& ip, const ServerWPtr& server );

	void Remove();

	void SendGreeting();
	void SendSyntaxError();
	void SendNotLoggedIn();
	void SendDataConnectionBusy();
	void SendSyst();

	bool AwaitLogin();
	PassState AwaitPassword();
	void AwaitReady();

	void HandleMode( const Command& cmd );
	void HandleType( const Command& cmd );
	void HandleStru( const Command& cmd );
	void HandlePort( const Command& cmd );
	void HandleRetr( const Command& cmd );
	void HandleStor( const Command& cmd );
	void HandleAbor();
	void HandleList( const Command& cmd );
	void HandlePasv( const Command& cmd );
	void HandleDele( const Command& cmd );
	void HandleMkd( const Command& cmd );
	void HandleRmd( const Command& cmd );

	void PrintDirectory();
	void ChangeDirectory( const Command& cmd );
	void ChangeDirectory( const std::string& cmd );

	void Upload( const Command& cmd );
	void Download( const Command& cmd );

	bool OpenDataConnection();

	Command GetCommand() { return ParseCommand( m_control->GetBuf() ); }


	TelnetPtr m_control;
	int m_controlSock;
	int m_listenSock;

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
	ServerWPtr m_server;

	std::string m_user;
	std::string m_ip;
};

#endif
