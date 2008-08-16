#ifndef __DFTPD__SESSIONCONTROLLERPTR_HPP__
#define __DFTPD__SESSIONCONTROLLERPTR_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class SessionController;
typedef boost::shared_ptr<SessionController> SessionControllerPtr;
typedef boost::weak_ptr<SessionController> SessionControllerWPtr;

#endif
