#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "AuthNone.hpp"
#include "AuthToken.hpp"
#include "Exceptions.hpp"
#include "LogNull.hpp"
#include "QtMain.hpp"
#include "Server.hpp"

Log* g_log = NULL;

QtApp::QtApp( QApplication* app )
	: m_app( app )
	, m_authenticationEnabled( true )
{
	g_log = this;
}

QtApp::~QtApp()
{
	g_log = new LogNull;
}

int QtApp::Run()
{
	QWidget* widget = new QWidget;
	setCentralWidget( widget );

	show();
	setWindowTitle( "Dump FTP server" );

	m_logbox = new QTextEdit();
	m_logbox->setReadOnly( true );
	m_logbox->setTextInteractionFlags( Qt::NoTextInteraction );

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget( m_logbox );
	widget->setLayout( layout );

	QAction* token = new QAction( "Generate token", this );
	connect( token, SIGNAL( triggered() ), this, SLOT( GenerateToken() ) );
	QAction* none = new QAction( "Disable authentication", this );
	connect( none, SIGNAL( triggered() ), this, SLOT( DisableAuthentication() ) );
	QAction* about = new QAction( "About", this );
	connect( about, SIGNAL( triggered() ), this, SLOT( About() ) );
	QAction* help = new QAction( "Help", this );
	connect( help, SIGNAL( triggered() ), this, SLOT( Help() ) );

#ifdef Q_WS_MAEMO_5
	QMenu* menu = menuBar()->addMenu( "" );
#else
	QMenu* menu = menuBar()->addMenu( "Actions" );
#endif
	menu->addAction( token );
	menu->addAction( none );
	menu->addAction( help );
	menu->addAction( about );

	m_timer = new QTimer( this );
	connect( m_timer, SIGNAL( timeout() ), this, SLOT( TimerTick() ) );
	m_timer->start( 10 );

	try
	{
		m_auth.reset( new AuthToken );
		m_server = Server::Create( m_auth );

		int sock = socket( AF_INET, SOCK_DGRAM, 0 );

		char buf[1024];
		struct ifconf ifc;
		ifc.ifc_len = sizeof( buf );
		ifc.ifc_buf = buf;

		ioctl( sock, SIOCGIFCONF, &ifc );

		struct ifreq* ifr = ifc.ifc_req;
		int num = ifc.ifc_len / sizeof( struct ifreq );

		g_log->Print( "Network interface list:" );
		while( num-- )
		{
			g_log->Print( std::string( ifr->ifr_name ) + ": " + inet_ntoa(((struct sockaddr_in *)&ifr->ifr_addr)->sin_addr) );
			ifr++;
		}

		::close( sock );
	}
	catch( ServerCrash& e )
	{
		ServerCrashed();
	}

	return m_app->exec();
}

void QtApp::Print( const std::string& text )
{
	m_logbox->append( QString( text.c_str() ) );
}

void QtApp::ServerCrashed()
{
	Print( "Server crashed" );

	m_server.reset();
	m_timer->stop();
}

void QtApp::TimerTick()
{
	try
	{
		m_server->Tick();
	}
	catch( ServerCrash& e )
	{
		ServerCrashed();
	}
}

void QtApp::About()
{
	QMessageBox::about( this, "Dumb FTP server", "Made by Bartosz Taudul\nemail: wolf.pld@gmail.com\nhttp://repo.or.cz/w/dftpd.git\nhttp://zgredowo.blogspot.com" );
}

void QtApp::Help()
{
	QMessageBox::about( this, "Usage instructions", "To be able to log-in after starting the server you will need to either disable authentication "
		       					"(any user and password will do, very insecure) or generate an one-time token that is presumably "
							"only known to you and which will be invalidated after successfully using it as a password (any "
							"username is accepted).\n\nPlease keep in mind that this server is highly experimental and by design "
							"doesn't support extravagant FTP commands. You won't be able to transmit EBCDIC encoded 39-bit byte "
							"record-structured files with telnet format controls in block mode to your DEC TOPS-20s mainframe. "
							"But you will be able to send and receive photos, videos, programs, etc." );
}

void QtApp::GenerateToken()
{
	if( m_server )
	{
		if( !m_authenticationEnabled )
		{
			g_log->Print( "Enabling authentication" );
			m_auth.reset( new AuthToken );
			m_server->SetAuth( m_auth );
			m_authenticationEnabled = true;
		}
		((AuthToken*)m_auth.get())->GenerateToken();
	}
}

void QtApp::DisableAuthentication()
{
	if( m_server && m_authenticationEnabled )
	{
		g_log->Print( "Authentication disabled" );
		m_auth.reset( new AuthNone );
		m_server->SetAuth( m_auth );
		m_authenticationEnabled = false;
	}
}

int main( int argc, char** argv )
{
	QtApp app( new QApplication( argc, argv ) );
	return app.Run();
}
