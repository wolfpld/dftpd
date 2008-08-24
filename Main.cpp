#include <iostream>
#include <unistd.h>
#include <signal.h>
#include "Server.hpp"
#include "LogSTDOUT.hpp"
#include "AuthNone.hpp"
#include "Exceptions.hpp"

bool g_exitRequested = false;
Log* g_log = new LogSTDOUT;

void RequestExit( int signum )
{
	g_exitRequested = true;
}

void Run()
{
	ServerPtr server( Server::Create( AuthPtr( new AuthNone ) ) );

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
	catch( ServerCrash& e )
	{
		g_log->Print( "Server crashed" );
		delete g_log;
		return 1;
	}

	delete g_log;
	return 0;
}
