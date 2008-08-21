#include <string.h>
#include "DataBufferListing.hpp"

static const char CRLF[] = { 13, 10, 0 };

DataBufferListing::DataBufferListing( const std::list<std::string>& list )
	: m_offset( 0 )
{
	for( std::list<std::string>::const_iterator it = list.begin(); it != list.end(); ++it )
	{
		m_buf += *it + CRLF;
	}
}

DataBufferListing::~DataBufferListing()
{
}

int DataBufferListing::Read( void* ptr, int size )
{
	std::string data = m_buf.substr( m_offset, size );
	size = data.size();

	if( size != 0 )
	{
		strncpy( (char*)ptr, data.c_str(), size );
		m_offset += size;
	}

	return size;
}
