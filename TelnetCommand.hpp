#ifndef __DFTPD__TELNETCOMMAND_HPP__
#define __DFTPD__TELNETCOMMAND_HPP__

#include <boost/shared_ptr.hpp>
#include "TelnetPtr.hpp"

class TelnetCommand
{
public:
	TelnetCommand( const TelnetWPtr& parent );
	~TelnetCommand();

	void Reset();

private:
	// RFC 854 specifies that commands are either 2 or 3 byte sequences.
	unsigned char m_buf[3];
	int m_byte;

	TelnetWPtr m_parent;
};

typedef boost::shared_ptr<TelnetCommand> TelnetCommandPtr;

#endif
