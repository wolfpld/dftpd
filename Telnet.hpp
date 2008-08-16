#ifndef __DFTPD__TELNET_HPP__
#define __DFTPD__TELNET_HPP__

#include <boost/shared_ptr.hpp>

class Telnet
{
public:
	Telnet( int sock );
	~Telnet();

private:
	// Owned by Session, not by Telnet
	int m_sock;
};

typedef boost::shared_ptr<Telnet> TelnetPtr;

#endif
