#ifndef __DFTPD__TELNETCOMMAND_HPP__
#define __DFTPD__TELNETCOMMAND_HPP__

#include <boost/shared_ptr.hpp>

class TelnetCommand
{
public:
	TelnetCommand();
	~TelnetCommand();

	void Reset();

private:
	// RFC 854 specifies that commands are either 2 or 3 byte sequences.
	// We can safely ignore the first bit (always 255).
	char m_buf[2];
};

typedef boost::shared_ptr<TelnetCommand> TelnetCommandPtr;

#endif
