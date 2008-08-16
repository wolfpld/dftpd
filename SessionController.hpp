#ifndef __DFTPD__SESSIONCONTROLLER_HPP__
#define __DFTPD__SESSIONCONTROLLER_HPP__

#include <list>
#include <queue>
#include <boost/shared_ptr.hpp>
#include "Session.hpp"

class SessionController
{
public:
	SessionController();
	~SessionController();

	void Tick();

	void Add( const SessionPtr& session );
	void Remove( const SessionPtr& session );

private:
	std::list<SessionPtr> m_list;
	std::queue<SessionPtr> m_removeList;
};

typedef boost::shared_ptr<SessionController> SessionControllerPtr;

#endif
