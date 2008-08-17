#ifndef __DFTPD__DATA_HPP__
#define __DFTPD__DATA_HPP__

#include <boost/shared_ptr.hpp>

class Data
{
public:
	Data();
	~Data();

private:
	int m_sock;
};

typedef boost::shared_ptr<Data> DataPtr;

#endif
