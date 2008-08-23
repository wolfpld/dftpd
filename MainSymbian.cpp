#include <iostream>
#include <string>
#include <unistd.h>
#include "Server.hpp"
#include "SymbianNetwork.hpp"
#include "LogSTDOUT.hpp"

Log* g_log = new LogSTDOUT;

void Run()
{
	std::string ip = EstablishConnection();

	ServerPtr server( Server::Create( ip ) );

	for(;;)
	{
		server->Tick();
	}
}

int main()
{
	try
	{
		Run();
	}
	catch( char const* err )
	{
		g_log->Print( std::string("Caught error: ") + err + "\nExiting" );
		usleep(5000000);
		delete g_log;
		return 1;
	}

	delete g_log;
	return 0;
}
