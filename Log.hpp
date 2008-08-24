#ifndef __DFTPD__LOG_HPP__
#define __DFTPD__LOG_HPP__

#include <string>

class Log
{
public:
	virtual void Print( const std::string& txt ) = 0;
};

extern Log* g_log;

#endif
