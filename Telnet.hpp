#ifndef __DFTPD__TELNET_HPP__
#define __DFTPD__TELNET_HPP__

#include <string>
#include "TelnetPtr.hpp"
#include "TelnetCommand.hpp"

class Telnet
{
	enum { BufSize = 1024 };

public:
	static TelnetPtr Create( int sock );
	~Telnet();

	bool Read();
	void Write( const std::string& msg );

	std::string GetBuf();

private:
	Telnet( int sock );

	// Owned by Session, not by Telnet
	int m_sock;

	std::string m_readBuf;
	TelnetCommandPtr m_cmd;
	TelnetWPtr m_this;
};

#endif
