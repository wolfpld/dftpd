#include <iostream>
#include "Listener.hpp"

int main()
{
	Listener l;

	std::cout << "Dumb FTP server\nIP: " << l.GetIPAddr() << std::endl;

	return 0;
}
