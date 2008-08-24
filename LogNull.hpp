#ifndef __DFTPD__LOGNULL_HPP__
#define __DFTPD__LOGNULL_HPP__

#include <string>
#include "Log.hpp"

class LogNull : public Log
{
public:
	void Print( const std::string& txt ) {};
};

#endif
