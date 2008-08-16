#include <iostream>
#include <unistd.h>
#include "Server.hpp"

void Run()
{
	Server server;

	for(;;)
	{
		server.Tick();
		usleep(10000);
	}
}

int main()
{
	try
	{
		Run();
	}
	catch( char* err )
	{
		std::cout << "Caught error: " << err << "\nExiting\n";
		return 1;
	}

	return 0;
}
