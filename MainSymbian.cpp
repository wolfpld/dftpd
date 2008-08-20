#include <iostream>
#include <string>
#include <unistd.h>
#include "Server.hpp"
#include "SymbianNetwork.hpp"

void Run()
{
	std::string ip = EstablishConnection();

	ServerPtr server( Server::Create( ip ) );

	for(;;)
	{
		server->Tick();
		usleep(10000);
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
		std::cout << "Caught error: " << err << "\nExiting\n";
		usleep(5000000);
		return 1;
	}

	return 0;
}
