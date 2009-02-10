#ifdef _WIN32
#include <winsock.h>
typedef int socklen_t;
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#endif

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "Data.hpp"
#include "Session.hpp"
#include "Exceptions.hpp"
#include "DataBufferFile.hpp"
#include "DataBufferListing.hpp"

#ifdef SYMBIAN

#include "DataBufferFileSymbian.hpp"

Data::Data( const SessionWPtr& session, RFile* file, Mode mode )
	: m_sock( 0 )
	, m_mode( mode )
	, m_session( session )
	, m_buf( new char[BufSize] )
{
	if( mode == M_UPLOAD )
	{
		m_data.reset( new DataBufferFileSymbian( file, BufSize ) );
	}
	else
	{
		m_data.reset( new DataBufferFileSymbian( file, BufSize ) );
	}
}
#endif

Data::Data( const SessionWPtr& session, FILE* file, Mode mode )
	: m_sock( 0 )
	, m_mode( mode )
	, m_session( session )
	, m_buf( new char[BufSize] )
{
	if( mode == M_UPLOAD )
	{
		m_data.reset( new DataBufferFile( file, BufSize ) );
	}
	else
	{
		m_data.reset( new DataBufferFile( file, BufSize ) );
	}
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
#ifdef _WIN32
		closesocket( m_sock );
#else
		close( m_sock );
#endif
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
		m_session.lock()->DataConnectionFinished();
	}
	else
	{
		int pos = 0;
		char *ptr = m_buf;

		while( pos != len && CanSend() )
		{
			int size = send( m_sock, ptr, len - pos, 0 );

			if( size == -1 )
			{
				throw SessionErrorException;
			}
			else if( size == 0 )
			{
				m_session.lock()->DataConnectionError();
				return;
			}

			pos += size;
			ptr += size;
		}

		if( pos != len )
		{
			m_data->Store( ptr, len - pos );
		}
	}
}

void Data::Receive()
{
	int total = 0;

	while( total < BufSize && CanReceive() )
	{
		int size = recv( m_sock, m_buf, BufSize - total, 0 );
		if( size == -1 )
		{
			throw SessionErrorException;
		}
		else if( size == 0 )
		{
			m_session.lock()->DataConnectionFinished();
			return;
		}
		else
		{
			int writeSize = m_data->Write( m_buf, size );

			if( writeSize != size )
			{
				m_session.lock()->OutOfSpace();
				return;
			}
		}

		total += size;
	}
}

bool Data::CanSend()
{
	fd_set fd;
	FD_ZERO( &fd );
	FD_SET( m_sock, &fd );
	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	select( m_sock + 1, NULL, &fd, NULL, &tv );

	return FD_ISSET( m_sock, &fd );
}

bool Data::CanReceive()
{
	fd_set fd;
	FD_ZERO( &fd );
	FD_SET( m_sock, &fd );
	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	select( m_sock + 1, &fd, NULL, NULL, &tv );

	return FD_ISSET( m_sock, &fd );
}
