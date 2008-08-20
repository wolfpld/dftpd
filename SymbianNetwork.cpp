#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sockio.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include "SymbianNetwork.hpp"

void EstablishConnection()
{
	// Open connection connection, otherwise we'll have no network access
	int sock = socket( PF_INET, SOCK_STREAM, 0 );

	sockaddr_in sa;

	sa.sin_family = AF_INET;
	sa.sin_port = htons( 80 );
	sa.sin_addr.s_addr = inet_addr( "147.243.3.83" );	// www.nokia.com
	memset( sa.sin_zero, 0, sizeof( sa.sin_zero ) );

	connect( sock, (sockaddr*)&sa, sizeof( sockaddr ) );

	close( sock );
}
