#ifndef __DFTPD__LOGSTDOUT_HPP__
#define __DFTPD__LOGSTDOUT_HPP__

#include <iostream>
#include "Log.hpp"

class LogSTDOUT : public Log
{
public:
	void Print( const std::string& txt ) const;
};

#endif
