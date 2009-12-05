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

#ifdef Q_WS_MAEMO_5
	QMenu* menu = menuBar()->addMenu( "" );
#else
	QMenu* menu = menuBar()->addMenu( "Actions" );
#endif
	menu->addAction( token );
	menu->addAction( none );
	menu->addAction( about );

	m_timer = new QTimer( this );
	connect( m_timer, SIGNAL( timeout() ), this, SLOT( TimerTick() ) );
	m_timer->start( 10 );

	try
	{
		m_auth.reset( new AuthToken );
		m_server = Server::Create( m_auth );
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
