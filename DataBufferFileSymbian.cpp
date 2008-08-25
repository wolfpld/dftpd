#include "DataBufferFileSymbian.hpp"

DataBufferFileSymbian::DataBufferFileSymbian( RFile* f, int secondaryBufferSize )
	: m_file( f )
	, m_secBuf( new char[secondaryBufferSize] )
	, m_secBufSize( 0 )
{
}

DataBufferFileSymbian::~DataBufferFileSymbian()
{
	delete[] m_secBuf;

	m_file->Close();
	delete m_file;
}

int DataBufferFileSymbian::Read( void* ptr, int size )
{
	int bufSize = m_secBufSize;
	m_secBufSize = 0;

	memcpy( ptr, m_secBuf, bufSize );

	if( size - bufSize == 0 )
	{
		return size;
	}

	TPtr8 buf( reinterpret_cast<TUint8*>( (char*)ptr + bufSize ), 0, size - bufSize );

	m_file->Read( buf, size - bufSize );

	return buf.Length() + bufSize;
}

int DataBufferFileSymbian::Write( void* ptr, int size )
{
	TPtr8 buf( reinterpret_cast<TUint8*>( ptr ), size, size );

	m_file->Write( buf, size );

	return size;
}

void DataBufferFileSymbian::Store( void* ptr, int size )
{
	memcpy( m_secBuf, ptr, size );
	m_secBufSize = size;
}
