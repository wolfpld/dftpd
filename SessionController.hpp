#ifndef __DFTPD__SESSIONCONTROLLER_HPP__
#define __DFTPD__SESSIONCONTROLLER_HPP__

#include <list>
#include <boost/shared_ptr.hpp>
#include "Session.hpp"

class SessionController
{
public:
	SessionController();
	~SessionController();

	void Tick();

	void Add( const SessionPtr& session );

private:
	std::list<SessionPtr> m_list;
};

typedef boost::shared_ptr<SessionController> SessionControllerPtr;

#endif
