#include <string>

class Listener
{
public:
	Listener();
	~Listener();

	const std::string& GetIPAddr() const { return m_ipaddr; }

private:
	int m_sock;
	std::string m_ipaddr;
};
