#ifndef __DFTPD__SESSIONPTR_HPP__
#define __DFTPD__SESSIONPTR_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class Session;
typedef boost::shared_ptr<Session> SessionPtr;
typedef boost::weak_ptr<Session> SessionWPtr;

#endif
