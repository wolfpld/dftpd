#include "SessionController.hpp"

SessionController::SessionController()
{}

SessionController::~SessionController()
{
}

void SessionController::Tick( const std::list<int>& activeFds )
{
	for( std::list<SessionPtr>::iterator it = m_list.begin(); it != m_list.end(); ++it )
	{
		std::list<int> fds = (*it)->GetFds();

		bool work = false;
		for( std::list<int>::const_iterator af = activeFds.begin(); af != activeFds.end(); ++af )
		{
			for( std::list<int>::const_iterator f = fds.begin(); f != fds.end(); ++f )
			{
				if( abs( *f ) == *af )
				{
					work = true;
					break;
				}
			}
			if( work ) break;
		}

		if( work )
		{
			(*it)->Tick();
		}
	}

	while( !m_removeList.empty() )
	{
		unsigned int size = m_list.size();
		m_list.remove( m_removeList.front() );
		m_removeList.pop();
		if( m_list.size() != size - 1 )
		{
			throw "[SessionController] Requested removal of non-existant session";
		}
	}
}

void SessionController::Add( const SessionPtr& session )
{
	m_list.push_back( session );
}

void SessionController::Remove( const SessionPtr& session )
{
	m_removeList.push( session );
}

std::list<int> SessionController::GetFds() const
{
	std::list<int> ret;

	for( std::list<SessionPtr>::const_iterator it = m_list.begin(); it != m_list.end(); ++it )
	{
		std::list<int> fds = (*it)->GetFds();
		ret.insert( ret.end(), fds.begin(), fds.end() );
	}

	return ret;
}
