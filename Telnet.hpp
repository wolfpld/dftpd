#ifndef __DFTPD__TELNET_HPP__
#define __DFTPD__TELNET_HPP__

#include <string>
#include <boost/shared_ptr.hpp>

class Telnet
{
	enum { BufSize = 1024 };

public:
	Telnet( int sock );
	~Telnet();

	bool Read();
	void Write( const std::string& msg );

	std::string GetBuf();

private:
	// Owned by Session, not by Telnet
	int m_sock;

	std::string m_readBuf;
};

typedef boost::shared_ptr<Telnet> TelnetPtr;

#endif
