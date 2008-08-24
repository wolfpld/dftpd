#ifndef __DFTPD__DATABUFFERFILE_HPP__
#define __DFTPD__DATABUFFERFILE_HPP__

#include <stdio.h>
#include "DataBuffer.hpp"

class DataBufferFile : public DataBuffer
{
	enum { BufSize = 1048576 };

public:
	DataBufferFile( FILE* f, int secondaryBufferSize, Mode mode );
	~DataBufferFile();

	int Read( void* ptr, int size );
	int Write( void* ptr, int size );
	void Store( void* ptr, int size );

private:
	FILE* m_file;

	char* m_buf;
	int m_offset;

	char* m_secBuf;
	int m_secBufSize;

	Mode m_mode;
};

#endif
