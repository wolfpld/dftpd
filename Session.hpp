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
