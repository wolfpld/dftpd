#include <iostream>
#include "Listener.hpp"

void Run()
{
	Listener l;

	std::cout << "Dumb FTP server\nIP: " << l.GetIPAddr() << std::endl;

	l.Listen();
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
