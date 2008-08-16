#include "SessionController.hpp"

SessionController::SessionController()
{}

SessionController::~SessionController()
{
}

void SessionController::Tick()
{
	for( std::list<SessionPtr>::iterator it = m_list.begin(); it != m_list.end(); ++it )
	{
		(*it)->Tick();
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
