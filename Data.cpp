#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "Data.hpp"
#include "Session.hpp"
#include "Exceptions.hpp"
#include "DataBufferFile.hpp"
#include "DataBufferListing.hpp"

Data::Data( const SessionWPtr& session, FILE* file, Mode mode )
	: m_sock( 0 )
	, m_mode( mode )
	, m_session( session )
	, m_buf( new char[BufSize] )
	, m_data( new DataBufferFile )
{
}

Data::Data( const SessionWPtr& session, const std::list<std::string>& list )
	: m_sock( 0 )
	, m_mode( M_UPLOAD )
	, m_session( session )
	, m_buf( new char[BufSize] )
	, m_data( new DataBufferListing( list ) )
{
}

Data::~Data()
{
	delete[] m_buf;

	if( m_sock != 0 )
	{
		close( m_sock );
	}
}

bool Data::Connect( const std::string& addr, int port )
{
	if( ( m_sock = socket( PF_INET, SOCK_STREAM, 0 ) ) == -1 )
	{
		throw SessionErrorException;
	}

	sockaddr_in sa;

	sa.sin_family = AF_INET;
	sa.sin_port = htons( port );
	sa.sin_addr.s_addr = inet_addr( addr.c_str() );
	memset( sa.sin_zero, 0, sizeof( sa.sin_zero ) );

	return connect( m_sock, (sockaddr*)&sa, sizeof( sockaddr ) ) != -1;
}

bool Data::Accept( int sock )
{
	sockaddr_in addr;
	socklen_t size = sizeof( addr );

	int incoming = accept( sock, (sockaddr*)&addr, &size );

	if( incoming == -1 )
	{
		return false;
	}

	m_sock = incoming;

	return true;
}

void Data::Tick()
{
	if( m_mode == M_UPLOAD )
	{
		Send();
	}
	else
	{
		Receive();
	}
}

void Data::Send()
{
	int len = m_data->Read( m_buf, BufSize );

	if( len == 0 )
	{
		SessionPtr sptr = m_session.lock();
		if( !sptr )
		{
			throw "Data lost its Session";
		}
		sptr->DataConnectionFinished();
	}
	else
	{
		int pos = 0;
		char *ptr = m_buf;

		while( pos != len )
		{
			int size = send( m_sock, ptr, len - pos, 0 );

			if( size == -1 )
			{
				throw SessionErrorException;
			}
			else if( size == 0 )
			{
				SessionPtr sptr = m_session.lock();
				if( !sptr )
				{
					throw "Data lost its Session";
				}
				sptr->DataConnectionError();

				return;
			}

			pos += size;
			ptr += size;
		}
	}
}

void Data::Receive()
{
	int size = recv( m_sock, m_buf, BufSize, 0 );

	if( size == -1 )
	{
		throw SessionErrorException;
	}
	else if( size == 0 )
	{
		SessionPtr sptr = m_session.lock();
		if( !sptr )
		{
			throw "Data lost its Session";
		}
		sptr->DataConnectionFinished();
	}
	else
	{
		int writeSize = m_data->Write( m_buf, size );

		if( writeSize != size )
		{
			SessionPtr sptr = m_session.lock();
			if( !sptr )
			{
				throw "Data lost its Session";
			}
			sptr->OutOfSpace();
		}
	}
}
