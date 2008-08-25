#ifndef __DFTPD__DATABUFFERFILESYMBIAN_HPP__
#define __DFTPD__DATABUFFERFILESYMBIAN_HPP__

#include <f32file.h>
#include "DataBuffer.hpp"

class DataBufferFileSymbian : public DataBuffer
{
public:
	DataBufferFileSymbian( RFile* f, int secondaryBufferSize );
	~DataBufferFileSymbian();

	int Read( void* ptr, int size );
	int Write( void* ptr, int size );
	void Store( void* ptr, int size );

private:
	RFile* m_file;

	char* m_secBuf;
	int m_secBufSize;
};

#endif
