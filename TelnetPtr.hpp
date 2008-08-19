#ifndef __DFTPD__TELNETPTR_HPP__
#define __DFTPD__TELNETPTR_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class Telnet;
typedef boost::shared_ptr<Telnet> TelnetPtr;
typedef boost::weak_ptr<Telnet> TelnetWPtr;

#endif
