#include <iostream>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "Session.hpp"
#include "SessionController.hpp"

int Session::m_counter = 0;

Session::Session( int controlSock, const SessionControllerPtr& sessionController )
	: m_control( new Telnet( controlSock ) )
	, m_controlSock( controlSock )
	, m_id( m_counter++ )
	, m_sessionController( sessionController )
{
	std::cout << "[Session] Initializing session " << m_id << std::endl;

	// Set socket to non-blocking
	if( fcntl( m_controlSock, F_SETFL, O_NONBLOCK ) == -1 )
	{
		throw strerror( errno );
	}
}

Session::~Session()
{
	if( m_controlSock != 0 )
	{
		std::cout << "[Session] Closing control socket " << m_id << std::endl;
		close( m_controlSock );
	}
}

SessionPtr Session::Create( int controlSock, const SessionControllerPtr& sessionController )
{
	SessionPtr ret( new Session( controlSock, sessionController ) );
	ret->m_this = ret;

	return ret;
}

void Session::Tick()
{
	try
	{
		m_control->Read();
	}
	catch( ConnectionTerminated& e )
	{
		std::cout << "[Session] Connection " << m_id << " terminated\n";
		Remove();
	}
}

void Session::Remove()
{
	SessionControllerPtr sessionController = m_sessionController.lock();

	if( !sessionController )
	{
		throw "Session lost SessionController";
	}

	sessionController->Remove( m_this.lock() );
}
