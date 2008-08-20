#ifndef __DFTPD__LISTENER_HPP__
#define __DFTPD__LISTENER_HPP__

#include <string>
#include <boost/shared_ptr.hpp>
#include "ServerPtr.hpp"

class Listener
{
public:
	Listener();
	Listener( const std::string& ip );
	~Listener();

	void SetServer( const ServerWPtr& server ) { m_server = server; }

	void Listen();
	void Tick();

	const std::string& GetIPAddr() const { return m_ipaddr; }

private:
	int m_sock;
	std::string m_ipaddr;
	ServerWPtr m_server;
};

typedef boost::shared_ptr<Listener> ListenerPtr;

#endif
