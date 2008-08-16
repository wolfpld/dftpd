#include "SessionController.hpp"

SessionController::SessionController()
{}

SessionController::~SessionController()
{
}

void SessionController::Tick()
{
}

void SessionController::Add( const SessionPtr& session )
{
	m_list.push_back( session );
}
