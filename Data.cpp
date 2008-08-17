#include <unistd.h>
#include "Data.hpp"

Data::Data()
{
}

Data::~Data()
{
	if( m_sock != 0 )
	{
		close( m_sock );
	}
}
