#include <string.h>
#include "DataBufferFile.hpp"

DataBufferFile::DataBufferFile( FILE* f, int secondaryBufferSize, Mode mode )
	: m_file( f )
	, m_secBuf( new char[secondaryBufferSize] )
	, m_secBufSize( 0 )
	, m_mode( mode )
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
	return fwrite( ptr, 1, size, m_file );
}

void DataBufferFile::Store( void* ptr, int size )
{
	memcpy( m_secBuf, ptr, size );
	m_secBufSize = size;
}
