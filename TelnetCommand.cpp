#include <string.h>
#include "TelnetCommand.hpp"
#include "Telnet.hpp"

static char IAC[] = { 255, 0 };

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

bool TelnetCommand::Parse( unsigned char byte )
{
	if( m_byte > 2 )
	{
		return false;
	}

	m_buf[m_byte++] = byte;

	TelnetPtr parent = m_parent.lock();
	if( m_byte == 1 )
	{
		if( m_buf[0] != 255 )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else if( m_byte == 2 )
	{
		switch( m_buf[1] )
		{
		case 240:	// SE
		case 241:	// NOP
		case 242:	// Data Mark
		case 243:	// Break;
		case 244:	// Interrup Process
		case 245:	// Abort output
		case 250:	// SB
			Reset();
			break;

		case 246:	// Are You There
			parent->Write( "HELLO" );
			Reset();
			break;

		case 247:	// Erase character
			parent->EraseCharacter();
			Reset();
			break;

		case 248:	// Erase Line
			parent->EraseLine();
			Reset();
			break;

		case 251:	// WILL
		case 252:	// WON'T
		case 253:	// DO
		case 254:	// DON'T
			break;

		case 255:
			parent->Write( IAC );
			Reset();
			break;

		default:
			return false;
		}
	}
	else
	{
		char tmp[] = { 255, 0, m_buf[2], 0 };

		switch( m_buf[1] )
		{
		case 251:	// WILL
			tmp[1] = 254;	// DON'T
			parent->Write( tmp );
			break;

		case 252:	// WON'T
		case 254:	// DON'T
			break;

		case 253:	// DO
			tmp[1] = 252;	// WON'T
			parent->Write( tmp );
			break;

		default:
			return false;
		}

		Reset();
	}

	return true;
}
