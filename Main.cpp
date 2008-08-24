#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "Server.hpp"
#include "LogSTDOUT.hpp"

bool g_exitRequested = false;
Log* g_log = new LogSTDOUT;

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
		g_log->Print( std::string("Caught error: ") + err );
		g_log->Print( "Exiting" );
		delete g_log;
		return 1;
	}

	delete g_log;
	return 0;
}
