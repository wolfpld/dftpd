#ifndef __DFTPD__DATA_HPP__
#define __DFTPD__DATA_HPP__

#include <stdio.h>
#include <string>
#include <boost/shared_ptr.hpp>
#include "SessionPtr.hpp"

class Data
{
	enum { BufSize = 65536 };

public:
	enum Mode
	{
		M_UPLOAD,
		M_DOWNLOAD
	};

	Data( const SessionWPtr& session, FILE* file, Mode mode );
	~Data();

	bool Connect( const std::string& addr, int port );
	void Tick();

private:
	void Send();
	void Receive();

	int m_sock;
	FILE* m_file;
	Mode m_mode;
	SessionWPtr m_session;
};

typedef boost::shared_ptr<Data> DataPtr;

#endif
