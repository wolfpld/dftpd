#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Server.hpp"

void Run()
{
	ServerPtr server( Server::Create() );

	for(;;)
	{
		server->Tick();
		usleep(10000);
	}
}

int main()
{
	// Try to establish connection, otherwise we'll have no network access
	int sock = socket( PF_INET, SOCK_STREAM, 0 );

	sockaddr_in sa;

	sa.sin_family = AF_INET;
	sa.sin_port = htons( 80 );
	sa.sin_addr.s_addr = inet_addr( "147.243.3.83" );	// www.nokia.com
	memset( sa.sin_zero, 0, sizeof( sa.sin_zero ) );

	connect( sock, (sockaddr*)&sa, sizeof( sockaddr ) );

	close( sock );

	try
	{
		Run();
	}
	catch( char const* err )
	{
		std::cout << "Caught error: " << err << "\nExiting\n";
		usleep(5000000);
		return 1;
	}

	return 0;
}
