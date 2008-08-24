#ifndef __DFTPD__DATABUFFERFILE_HPP__
#define __DFTPD__DATABUFFERFILE_HPP__

#include <stdio.h>
#include <pthread.h>
#include "DataBuffer.hpp"

class DataBufferFile : public DataBuffer
{
	enum { BufSize = 131072 };

public:
	DataBufferFile( FILE* f, int secondaryBufferSize, Mode mode );
	~DataBufferFile();

	int Read( void* ptr, int size );
	int Write( void* ptr, int size );
	void Store( void* ptr, int size );

private:
	void SaveBuffer();
	static void* SaveBuffer( void* ptr );

	FILE* m_file;

	char* m_buf;
	char* m_buf2;
	int m_offset;
	int m_offsetToWrite;

	char* m_secBuf;
	int m_secBufSize;

	Mode m_mode;

	pthread_t m_thread;
	bool m_threadRunning;
};

#endif
