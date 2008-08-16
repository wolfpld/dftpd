#ifndef __DFTPD__SERVERPTR_HPP__
#define __DFTPD__SERVERPTR_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class Server;
typedef boost::shared_ptr<Server> ServerPtr;
typedef boost::weak_ptr<Server> ServerWPtr;

#endif
