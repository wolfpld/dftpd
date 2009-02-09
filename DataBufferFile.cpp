#include <string.h>
#include "DataBufferFile.hpp"

DataBufferFile::DataBufferFile( FILE* f, int secondaryBufferSize )
	: m_file( f )
	, m_secBuf( new char[secondaryBufferSize] )
	, m_secBufSize( 0 )
{
}

DataBufferFile::~DataBufferFile()
{
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

int DataBufferFile::Write( void* ptr, int size )
{
	return fwrite( ptr, size, 1, m_file );
}

void DataBufferFile::Store( void* ptr, int size )
{
	memcpy( m_secBuf, ptr, size );
	m_secBufSize = size;
}
