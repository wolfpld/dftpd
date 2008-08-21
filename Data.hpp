#ifndef __DFTPD__DATA_HPP__
#define __DFTPD__DATA_HPP__

#include <stdio.h>
#include <string>
#include <list>
#include <boost/shared_ptr.hpp>
#include "SessionPtr.hpp"
#include "DataBuffer.hpp"

class Data
{
	enum { BufSize = 65536 };

public:
	enum Mode
	{
		M_UPLOAD,
		M_DOWNLOAD,
	};

	Data( const SessionWPtr& session, FILE* file, Mode mode );
	Data( const SessionWPtr& session, const std::list<std::string>& list );
	~Data();

	bool Connect( const std::string& addr, int port );
	bool Accept( int sock );

	void Tick();

	int GetSock() const { return m_sock; }
	Mode GetMode() const { return m_mode; }

private:
	void Send();
	void Receive();

	bool CanSend();
	bool CanReceive();

	int m_sock;
	Mode m_mode;
	SessionWPtr m_session;
	char* m_buf;
	DataBufferPtr m_data;
};

typedef boost::shared_ptr<Data> DataPtr;

#endif
