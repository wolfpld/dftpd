#ifndef __DFTPD__DATABUFFERFILE_HPP__
#define __DFTPD__DATABUFFERFILE_HPP__

#include "DataBuffer.hpp"

class DataBufferFile : public DataBuffer
{
public:
	DataBufferFile();
	~DataBufferFile();

	int Read( void* ptr, int size );
	int Write( void* ptr, int size );

private:
};

#endif
