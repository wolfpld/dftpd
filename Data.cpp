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

static const char CRLF[] = { 13, 10, 0 };

Data::Data( const SessionWPtr& session, FILE* file, Mode mode )
	: m_sock( 0 )
	, m_file( file )
	, m_mode( mode )
	, m_session( session )
{
	m_buf = new char[BufSize];
}

Data::Data( const SessionWPtr& session, const std::vector<std::string>& list )
	: m_sock( 0 )
	, m_file( NULL )
	, m_mode( M_LISTING )
	, m_list( list )
	, m_iter( m_list.begin() )
	, m_session( session )
{
	m_buf = new char[BufSize];
}

Data::~Data()
{
	delete[] m_buf;

	if( m_file != NULL )
	{
		fclose( m_file );
	}

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
	else if( m_mode == M_DOWNLOAD )
	{
		Receive();
	}
	else
	{
		SendList();
	}
}

void Data::Send()
{
	int len = fread( m_buf, 1, BufSize, m_file );

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
		int writeSize = fwrite( m_buf, 1, size, m_file );

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

void Data::SendList()
{
	if( m_iter == m_list.end() )
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
		std::string data = *m_iter + CRLF;
		unsigned int pos = 0;
		char *ptr = (char*)data.c_str();

		while( pos != data.size() )
		{
			int size = send( m_sock, ptr, data.size() - pos, 0 );

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

		++m_iter;
	}
}
