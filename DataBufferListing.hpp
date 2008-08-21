#ifndef __DFTPD__DATABUFFERLISTING_HPP__
#define __DFTPD__DATABUFFERLISTING_HPP__

#include <string>
#include <list>
#include "DataBuffer.hpp"

class DataBufferListing : public DataBuffer
{
public:
	DataBufferListing( const std::list<std::string>& list );
	~DataBufferListing();

	int Read( void* ptr, int size );

private:
	std::string m_buf;
	int m_offset;
};

#endif
