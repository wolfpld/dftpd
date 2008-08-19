#include <iostream>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <boost/lexical_cast.hpp>
#include "Session.hpp"
#include "SessionController.hpp"
#include "String.hpp"
#include "Exceptions.hpp"

int Session::m_counter = 0;

Session::Session( int controlSock, const SessionControllerPtr& sessionController, const AuthPtr& auth, const std::string& ip )
	: m_control( Telnet::Create( controlSock ) )
	, m_controlSock( controlSock )
	, m_listenSock( 0 )
	, m_dataPort( 20 )
	, m_id( m_counter++ )
	, m_state( S_GREETING )
	, m_sessionController( sessionController )
	, m_auth( auth )
	, m_ip( ip )
{
	sockaddr_in addr;
	socklen_t size = sizeof( sockaddr_in );

	if( getpeername( m_controlSock, (sockaddr*)&addr, &size ) == -1 )
	{
		throw strerror( errno );
	}

	m_dataAddress = inet_ntoa( addr.sin_addr );

	std::cout << "[Session] Initializing session " << m_id << " for " << m_dataAddress << std::endl;

	// Set socket to non-blocking
	if( fcntl( m_controlSock, F_SETFL, O_NONBLOCK ) == -1 )
	{
		throw strerror( errno );
	}
}

Session::~Session()
{
	if( m_controlSock != 0 )
	{
		std::cout << "[Session] Closing control socket " << m_id << std::endl;
		close( m_controlSock );
	}

	if( m_listenSock != 0 )
	{
		close( m_listenSock );
	}
}

SessionPtr Session::Create( int controlSock, const SessionControllerPtr& sessionController, const AuthPtr& auth, const std::string& ip )
{
	SessionPtr ret( new Session( controlSock, sessionController, auth, ip ) );
	ret->m_this = ret;

	return ret;
}

void Session::Tick()
{
	try
	{
		try
		{
			switch( m_state )
			{
			case S_GREETING:
				SendGreeting();
				m_state = S_LOGIN;
				break;

			case S_LOGIN:
				if( AwaitLogin() )
				{
					m_state = S_PASSWORD;
				}
				break;

			case S_PASSWORD:
			{
				PassState state = AwaitPassword();

				if( state == PS_LOGGEDIN )
				{
					m_state = S_READY;
				}
				else if( state == PS_BADPASS )
				{
					m_state = S_LOGIN;
				}
				break;
			}

			case S_READY:
				AwaitReady();
				break;

			default:
				break;
			}

			if( m_data )
			{
				m_data->Tick();
			}
		}
		catch( SyntaxError& e )
		{
			SendSyntaxError();
		}
		catch( QuitRequested& e )
		{
			m_control->Write( "221 Bye" );
			Remove();
		}
	}
	catch( ConnectionTerminated& e )
	{
		std::cout << "[Session] Connection " << m_id << " terminated\n";
		Remove();
	}
}

void Session::Remove()
{
	SessionControllerPtr sessionController = m_sessionController.lock();

	if( !sessionController )
	{
		throw "Session lost SessionController";
	}

	sessionController->Remove( m_this.lock() );
}

void Session::SendGreeting()
{
	m_control->Write( "220 Dumb FTP Server ready" );
}

void Session::SendSyntaxError()
{
	m_control->Write( "500 Syntax error" );
}

void Session::SendNotLoggedIn()
{
	m_control->Write( "530 Not logged in" );
}

void Session::SendDataConnectionBusy()
{
	// Is it the correct answer?
	m_control->Write( "425 File transfer already takes place" );
}

void Session::SendSyst()
{
	// Is it even relevant?
	m_control->Write( "215 UNIX" );
}

bool Session::AwaitLogin()
{
	if( m_control->Read() )
	{
		Command cmd = GetCommand();

		if( cmd[0] == "USER" )
		{
			if( cmd.size() != 2 )
			{
				SendSyntaxError();
				return false;
			}

			if( m_auth->Login( cmd[1] ) )
			{
				m_control->Write( "331 Need password" );
				m_user = cmd[1];
				return true;
			}
			else
			{
				SendNotLoggedIn();
				return false;
			}
		}
		else if( cmd[0] == "QUIT" )
		{
			throw QuitRequestedException;
		}
		else
		{
			SendNotLoggedIn();
		}
	}

	return false;
}

Session::PassState Session::AwaitPassword()
{
	if( m_control->Read() )
	{
		Command cmd = GetCommand();

		if( cmd[0] == "PASS" )
		{
			if( cmd.size() != 2 )
			{
				SendSyntaxError();
				return PS_BADPASS;
			}

			if( m_auth->Password( m_user, cmd[1] ) )
			{
				m_control->Write( "230 Logged in" );

				m_filesystem.reset( new Filesystem( m_auth->GetRoot( m_user ) ) );

				std::cout << "[Session] User " << m_user << " logged in on session " << m_id << std::endl;

				return PS_LOGGEDIN;
			}
			else
			{
				SendNotLoggedIn();
				return PS_BADPASS;
			}
		}
		else if( cmd[0] == "QUIT" )
		{
			throw QuitRequestedException;
		}
		else
		{
			SendNotLoggedIn();
			return PS_BADPASS;
		}
	}

	return PS_NONE;
}

void Session::AwaitReady()
{
	if( m_control->Read() )
	{
		Command cmd = GetCommand();

		if( cmd[0] == "QUIT" )
		{
			throw QuitRequestedException;
		}
		else if( cmd[0] == "NOOP" )
		{
			m_control->Write( "200 OK" );
		}
		else if( cmd[0] == "MODE" )
		{
			HandleMode( cmd );
		}
		else if( cmd[0] == "TYPE" )
		{
			HandleType( cmd );
		}
		else if( cmd[0] == "STRU" )
		{
			HandleStru( cmd );
		}
		else if( cmd[0] == "PWD" )
		{
			PrintDirectory();
		}
		else if( cmd[0] == "CWD" )
		{
			ChangeDirectory( cmd );
		}
		else if( cmd[0] == "CDUP" )
		{
			ChangeDirectory( ".." );
		}
		else if( cmd[0] == "SYST" )
		{
			SendSyst();
		}
		else if( cmd[0] == "PORT" )
		{
			HandlePort( cmd );
		}
		else if( cmd[0] == "RETR" )
		{
			HandleRetr( cmd );
		}
		else if( cmd[0] == "STOR" )
		{
			HandleStor( cmd );
		}
		else if( cmd[0] == "ABOR" )
		{
			HandleAbor();
		}
		else if( cmd[0] == "LIST" )
		{
			HandleList( cmd );
		}
		else if( cmd[0] == "PASV" )
		{
			HandlePasv( cmd );
		}
		else
		{
			throw SyntaxErrorException;
		}
	}
}

void Session::HandleMode( const Command& cmd )
{
	if( cmd.size() != 2 )
	{
		throw SyntaxErrorException;
	}

	std::string param = cmd[1];
	ToUpper( param );

	if( param == "S" )
	{
		m_control->Write( "200 OK" );
	}
	else if( param == "B" || param == "C" )
	{
		m_control->Write( "504 Not implemented" );
	}
	else
	{
		throw SyntaxErrorException;
	}
}

void Session::HandleType( const Command& cmd )
{
	if( cmd.size() < 2 || cmd.size() > 3)
	{
		throw SyntaxErrorException;
	}

	std::string param = cmd[1];
	ToUpper( param );

	if( param == "A" )
	{
		if( cmd.size() == 3 )
		{
			std::string param2 = cmd[2];
			ToUpper( param2 );

			if( param2 != "N" )
			{
				m_control->Write( "504 Not implemented" );
				return;
			}
		}

		m_control->Write( "200 OK" );
	}
	else if( param == "I" )
	{
		m_control->Write( "200 OK" );
	}
	else if( param == "E" || param == "L" )
	{
		m_control->Write( "504 Not implemented" );
	}
	else
	{
		throw SyntaxErrorException;
	}
}

void Session::HandleStru( const Command& cmd )
{
	if( cmd.size() != 2 )
	{
		throw SyntaxErrorException;
	}

	std::string param = cmd[1];
	ToUpper( param );

	if( param == "F" )
	{
		m_control->Write( "200 OK" );
	}
	else if( param == "R" )
	{
		throw "STRU R not implemented";
	}
	else if( param == "P" )
	{
		m_control->Write( "504 Not implemented" );
	}
	else
	{
		throw SyntaxErrorException;
	}
}

void Session::HandlePort( const Command& cmd )
{
	if( cmd.size() != 2 )
	{
		throw SyntaxErrorException;
	}

	if( m_listenSock )
	{
		close( m_listenSock );
		m_listenSock = 0;
	}

	PortVector pv = SplitPort( cmd[1] );
	if( pv.size() != 6 )
	{
		throw SyntaxErrorException;
	}

	m_dataAddress = pv[0] + "." + pv[1] + "." + pv[2] + "." + pv[3];
	m_dataPort = ( boost::lexical_cast<int>( pv[4] ) << 8 ) + boost::lexical_cast<int>( pv[5] );

	m_control->Write( "200 OK" );
}

void Session::HandleRetr( const Command& cmd )
{
	if( m_data )
	{
		SendDataConnectionBusy();
	}
	else
	{
		Upload( cmd );
	}
}

void Session::HandleStor( const Command& cmd )
{
	if( m_data )
	{
		SendDataConnectionBusy();
	}
	else
	{
		Download( cmd );
	}
}

void Session::HandleAbor()
{
	if( !m_data )
	{
		m_control->Write( "225 No data connection" );
	}
	else
	{
		std::cout << "[Session] Data connection aborted on session " << m_id << std::endl;

		m_data.reset();

		m_control->Write( "426 File transfer aborted" );
		m_control->Write( "226 Data connection closed" );
	}
}

void Session::HandleList( const Command& cmd )
{
	std::string path( "." );

	if( cmd.size() > 1 && cmd[1].size() > 0 && cmd[1][0] != '-' )
	{
		path = cmd[1];
	}

	std::vector<std::string> list = m_filesystem->GetListing( path );

	if( list.empty() )
	{
		m_control->Write( "450 Some problems" );
		return;
	}

	m_data.reset( new Data( m_this, list ) );

	if( !OpenDataConnection() )
	{
		m_control->Write( "425 Can't open data connection" );
		m_data.reset();
	}
	else
	{
		m_control->Write( std::string( "150 Listing " ) + path );
		std::cout << "[Session] Sending listing on session " << m_id << std::endl;
	}
}

void Session::HandlePasv( const Command& cmd )
{
	if( cmd.size() != 1 )
	{
		throw SyntaxErrorException;
	}

	if( !m_listenSock )
	{
		if( ( m_listenSock = socket( PF_INET, SOCK_STREAM, 0 ) ) == -1 )
		{
			throw strerror( errno );
		}

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = 0;
		inet_aton( m_ip.c_str(), &addr.sin_addr );
		memset( addr.sin_zero, 0, sizeof( addr.sin_zero ) );

		if( bind( m_listenSock, (sockaddr*)&addr, sizeof( addr ) ) == -1 )
		{
			throw strerror( errno );
		}

		if( listen( m_listenSock, 1 ) == -1 )
		{
			throw strerror( errno );
		}
	}

	sockaddr_in addr;
	socklen_t len = sizeof( addr );
	if( getsockname( m_listenSock, (sockaddr*)&addr, &len ) == -1 )
	{
		throw strerror( errno );
	}

	int port = ntohs( addr.sin_port );
	std::string ip = inet_ntoa( addr.sin_addr );

	std::replace( ip.begin(), ip.end(), '.', ',' );

	m_control->Write( std::string( "227 Entering passive mode " ) + ip +
			"," + boost::lexical_cast<std::string>( port >> 8 ) +
			"," + boost::lexical_cast<std::string>( port & 0xFF ) );
}

void Session::PrintDirectory()
{
	m_control->Write( std::string( "257 " ) + m_filesystem->GetPath() );
}

void Session::ChangeDirectory( const Command& cmd )
{
	if( cmd.size() != 2 )
	{
		throw SyntaxErrorException;
	}

	ChangeDirectory( cmd[1] );
}

void Session::ChangeDirectory( const std::string& cd )
{
	if( m_filesystem->ChangeDirectory( cd ) )
	{
		m_control->Write( std::string( "200 Changed directory to " ) + m_filesystem->GetPath() );
	}
	else
	{
		m_control->Write( "550 Requested action not taken" );
	}
}

void Session::Upload( const Command& cmd )
{
	FILE *f;

	if( cmd.size() != 2 )
	{
		throw SyntaxErrorException;
	}

	if( !m_filesystem->FileExists( cmd[1] ) )
	{
		m_control->Write( std::string( "550 File " ) + cmd[1] + " not found" );
		return;
	}
	if( ( f = m_filesystem->FileOpen( cmd[1], Filesystem::M_READ ) ) == NULL )
	{
		m_control->Write( std::string( "450 File " ) + cmd[1] + " not accessible" );
		return;
	}

	m_data.reset( new Data( m_this, f, Data::M_UPLOAD ) );

	if( !OpenDataConnection() )
	{
		m_control->Write( "425 Can't open data connection" );
		m_data.reset();
	}
	else
	{
		m_control->Write( std::string( "150 Sending " ) + cmd[1] );
		std::cout << "[Session] Opened new upload on session " << m_id << std::endl;
	}
}

void Session::Download( const Command& cmd )
{
	FILE *f;

	if( cmd.size() != 2 )
	{
		throw SyntaxErrorException;
	}

	if( ( f = m_filesystem->FileOpen( cmd[1], Filesystem::M_WRITE ) ) == NULL )
	{
		m_control->Write( std::string( "450 File " ) + cmd[1] + " not accessible" );
		return;
	}

	m_data.reset( new Data( m_this, f, Data::M_DOWNLOAD ) );

	if( !OpenDataConnection() )
	{
		m_control->Write( "425 Can't open data connection" );
		m_data.reset();
	}
	else
	{
		m_control->Write( std::string( "150 Receiving " ) + cmd[1] );
		std::cout << "[Session] Opened new download on session " << m_id << std::endl;
	}
}

void Session::DataConnectionFinished()
{
	std::cout << "[Session] Data connection closed on session " << m_id << std::endl;

	m_control->Write( "226 File transfer completed" );

	m_data.reset();
}

void Session::DataConnectionError()
{
	std::cout << "[Session] Data connection error on session " << m_id << std::endl;

	m_control->Write( "426 Data connection lost" );

	m_data.reset();
}

void Session::OutOfSpace()
{
	std::cout << "[Session] Out of space on session " << m_id << std::endl;

	m_control->Write( "552 No space left" );

	m_data.reset();
}

bool Session::OpenDataConnection()
{
	bool ok;

	if( m_listenSock )
	{
		ok = m_data->Accept( m_listenSock );
		close( m_listenSock );
		m_listenSock = 0;
	}
	else
	{
		ok = m_data->Connect( m_dataAddress, m_dataPort );
	}

	return ok;
}
