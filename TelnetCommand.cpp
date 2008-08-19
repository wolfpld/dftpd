#include <string.h>
#include "TelnetCommand.hpp"

TelnetCommand::TelnetCommand()
{
	Reset();
}

TelnetCommand::~TelnetCommand()
{
}

void TelnetCommand::Reset()
{
	memset( m_buf, 0, sizeof( m_buf ) );
}
