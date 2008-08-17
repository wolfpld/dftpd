#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "Data.hpp"

Data::Data( FILE* file, Mode mode )
	: m_sock( 0 )
	, m_file( file )
	, m_mode( mode )
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
}
