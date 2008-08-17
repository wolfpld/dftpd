#ifndef __DFTPD__DATA_HPP__
#define __DFTPD__DATA_HPP__

#include <string>
#include <boost/shared_ptr.hpp>

class Data
{
public:
	Data();
	~Data();

	bool Connect( const std::string& addr, int port );

private:
	int m_sock;
};

typedef boost::shared_ptr<Data> DataPtr;

#endif
