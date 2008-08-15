class Session
{
public:
	Session( int controlSock );
	~Session();

private:
	int m_controlSock;
};
