#include <string.h>
#include "DataBufferFile.hpp"

DataBufferFile::DataBufferFile( FILE* f, int secondaryBufferSize, Mode mode )
	: m_file( f )
	, m_buf( new char[BufSize] )
	, m_secBuf( new char[secondaryBufferSize] )
	, m_secBufSize( 0 )
	, m_mode( mode )
{
	if( mode == M_WRITE )
	{
		m_offset = 0;
	}
	else
	{
		m_offset = BufSize;
	}
}

DataBufferFile::~DataBufferFile()
{
	// Dump buffer to disk if needed
	if( m_mode == M_WRITE && m_offset != 0 )
	{
		fwrite( m_buf, 1, m_offset, m_file );
	}

	delete[] m_buf;
	delete[] m_secBuf;

	fclose( m_file );
}

int DataBufferFile::Read( void* ptr, int size )
{
	int bufSize = m_secBufSize;
	m_secBufSize = 0;

	memcpy( ptr, m_secBuf, bufSize );

	return fread( (char*)ptr + bufSize, 1, size - bufSize, m_file ) + bufSize;
}

int DataBufferFile::Write( void* _ptr, int _size )
{
	int size = _size;
	char* ptr = (char*)_ptr;

	while( size != 0 )
	{
		int dataToWrite = std::min( size, BufSize - m_offset );
		memcpy( m_buf + m_offset, ptr, dataToWrite );
		ptr += dataToWrite;
		m_offset += dataToWrite;
		size -= dataToWrite;

		if( m_offset == BufSize )
		{
			fwrite( m_buf, 1, BufSize, m_file );
			m_offset = 0;
		}
	}

	return _size;
}

void DataBufferFile::Store( void* ptr, int size )
{
	memcpy( m_secBuf, ptr, size );
	m_secBufSize = size;
}
