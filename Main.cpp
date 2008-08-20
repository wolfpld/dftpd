#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "Server.hpp"

bool g_exitRequested = false;

void RequestExit( int signum )
{
	g_exitRequested = true;
}

void Run()
{
	ServerPtr server( Server::Create() );

	while( !g_exitRequested )
	{
		server->Tick();
	}
}

int main()
{
	signal( SIGINT, RequestExit );

	try
	{
		Run();
	}
	catch( char const* err )
	{
		std::cout << "Caught error: " << err << "\nExiting\n";
		return 1;
	}

	return 0;
}
