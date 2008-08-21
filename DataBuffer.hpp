#ifndef __DFTPD__DATABUFFER_HPP__
#define __DFTPD__DATABUFFER_HPP__

#include <boost/shared_ptr.hpp>

class DataBuffer
{
public:
	virtual ~DataBuffer() {}
	virtual int Read( void* ptr, int size ) { throw "Read not supported"; }
	virtual int Write( void* ptr, int size ) { throw "Write not supported"; }
	virtual void Store( void* ptr, int size ) { throw "Store not supported"; }

protected:
	DataBuffer() {}
};

typedef boost::shared_ptr<DataBuffer> DataBufferPtr;

#endif
