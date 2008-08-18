#ifndef __DFTPD__DATA_HPP__
#define __DFTPD__DATA_HPP__

#include <stdio.h>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "SessionPtr.hpp"

class Data
{
	enum { BufSize = 65536 };

public:
	enum Mode
	{
		M_UPLOAD,
		M_DOWNLOAD,
		M_LISTING
	};

	Data( const SessionWPtr& session, FILE* file, Mode mode );
	Data( const SessionWPtr& session, const std::vector<std::string>& list );
	~Data();

	bool Connect( const std::string& addr, int port );
	bool Accept( int sock );

	void Tick();

private:
	void Send();
	void Receive();
	void SendList();

	int m_sock;
	FILE* m_file;
	Mode m_mode;
	std::vector<std::string> m_list;
	std::vector<std::string>::const_iterator m_iter;
	SessionWPtr m_session;
};

typedef boost::shared_ptr<Data> DataPtr;

#endif
