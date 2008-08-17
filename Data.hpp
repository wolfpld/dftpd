#ifndef __DFTPD__DATA_HPP__
#define __DFTPD__DATA_HPP__

#include <stdio.h>
#include <string>
#include <boost/shared_ptr.hpp>

class Data
{
public:
	enum Mode
	{
		M_UPLOAD,
		M_DOWNLOAD
	};

	Data( FILE* file, Mode mode );
	~Data();

	bool Connect( const std::string& addr, int port );
	void Tick();

private:
	int m_sock;
	FILE* m_file;
	Mode m_mode;
};

typedef boost::shared_ptr<Data> DataPtr;

#endif
