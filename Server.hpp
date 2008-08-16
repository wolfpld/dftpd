#include "Listener.hpp"
#include "SessionController.hpp"

class Server
{
public:
	Server();
	~Server();

	void Tick();

private:
	ListenerPtr m_listener;
	SessionControllerPtr m_sessionController;
};
