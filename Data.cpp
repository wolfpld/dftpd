#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "Data.hpp"
#include "Session.hpp"

Data::Data( const SessionWPtr& session, FILE* file, Mode mode )
	: m_sock( 0 )
	, m_file( file )
	, m_mode( mode )
	, m_session( session )
{
}

Data::~Data()
{
	fclose( m_file );

	if( m_sock != 0 )
	{
		close( m_sock );
	}
}

bool Data::Connect( const std::string& addr, int port )
{
	if( ( m_sock = socket( PF_INET, SOCK_STREAM, 0 ) ) == -1 )
	{
		throw strerror( errno );
	}

	sockaddr_in sa;

	sa.sin_family = AF_INET;
	sa.sin_port = htons( port );
	sa.sin_addr.s_addr = inet_addr( addr.c_str() );
	memset( sa.sin_zero, 0, sizeof( sa.sin_zero ) );

	return connect( m_sock, (sockaddr*)&sa, sizeof( sockaddr ) ) != -1;
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
	char buf[BufSize];

	int len = fread( buf, 1, BufSize, m_file );

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
		char *ptr = buf;

		while( pos != len )
		{
			int size = send( m_sock, ptr, len - pos, 0 );

			if( size == -1 )
			{
				throw strerror( errno );
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
	char tmpBuf[BufSize];
	int size = recv( m_sock, tmpBuf, BufSize, 0 );

	if( size == -1 )
	{
		if( errno == EAGAIN )
		{
			return;
		}

		throw strerror( errno );
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
		int writeSize = fwrite( tmpBuf, 1, size, m_file );

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
