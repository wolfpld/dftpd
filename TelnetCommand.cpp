#include <string.h>
#include "TelnetCommand.hpp"

TelnetCommand::TelnetCommand( const TelnetWPtr& parent )
	: m_parent( parent )
{
	Reset();
}

TelnetCommand::~TelnetCommand()
{
}

void TelnetCommand::Reset()
{
	memset( m_buf, 0, sizeof( m_buf ) );
	m_byte = 0;
}
