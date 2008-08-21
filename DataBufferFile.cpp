#include "DataBufferFile.hpp"

DataBufferFile::DataBufferFile( FILE* f )
	: m_file( f )
{
}

DataBufferFile::~DataBufferFile()
{
	fclose( m_file );
}

int DataBufferFile::Read( void* ptr, int size )
{
	return fread( ptr, 1, size, m_file );
}

int DataBufferFile::Write( void* ptr, int size )
{
	return fwrite( ptr, 1, size, m_file );
}
