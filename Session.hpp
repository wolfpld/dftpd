#ifndef __DFTPD__SESSION_HPP__
#define __DFTPD__SESSION_HPP__

#include <boost/shared_ptr.hpp>
#include "SessionControllerPtr.hpp"

class Session
{
public:
	Session( int controlSock, const SessionControllerPtr& sessionController );
	~Session();

	void Tick();

private:
	int m_controlSock;
	int m_id;

	static int m_counter;

	SessionControllerWPtr m_sessionController;
};

typedef boost::shared_ptr<Session> SessionPtr;

#endif
