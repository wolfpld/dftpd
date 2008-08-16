#ifndef __DFTPD__SESSION_HPP__
#define __DFTPD__SESSION_HPP__

#include <boost/shared_ptr.hpp>

class Session
{
public:
	Session( int controlSock );
	~Session();

private:
	int m_controlSock;
};

typedef boost::shared_ptr<Session> SessionPtr;

#endif
