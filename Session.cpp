#include <iostream>
#include <string.h>
#include <errno.h>
#include "Session.hpp"

Session::Session( int controlSock )
	: m_controlSock( controlSock )
{}

Session::~Session()
{
	if( m_controlSock != 0 )
	{
		std::cout << "[Session] Closing control socket" << std::endl;
		if( close( m_controlSock ) == -1 )
		{
			throw strerror( errno );
		}
	}
}
