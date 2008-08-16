#ifndef __DFTPD__SESSION_HPP__
#define __DFTPD__SESSION_HPP__

#include "SessionPtr.hpp"
#include "SessionControllerPtr.hpp"
#include "Telnet.hpp"

class Session
{
public:
	static SessionPtr Create( int controlSock, const SessionControllerPtr& sessionController );
	~Session();

	void Tick();

private:
	Session( int controlSock, const SessionControllerPtr& sessionController );

	void Remove();

	TelnetPtr m_control;
	int m_controlSock;

	int m_id;
	static int m_counter;

	SessionControllerWPtr m_sessionController;
	SessionWPtr m_this;
};

#endif
