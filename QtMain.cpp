#include "AuthNone.hpp"
#include "AuthToken.hpp"
#include "Exceptions.hpp"
#include "LogNull.hpp"
#include "QtMain.hpp"
#include "Server.hpp"

Log* g_log = NULL;

QtApp::QtApp( int argc, char** argv )
	: m_app( new QApplication( argc, argv ) )
{
	g_log = this;
}

QtApp::~QtApp()
{
	g_log = new LogNull;
	delete m_app;
}

int QtApp::Run()
{
	QWidget window;
	window.show();
	window.setWindowTitle( "Dump FTP server" );

	m_logbox = new QTextEdit();
	m_logbox->setReadOnly( true );

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget( m_logbox );
	window.setLayout( layout );

	m_timer = new QTimer( this );
	connect( m_timer, SIGNAL( timeout() ), this, SLOT( TimerTick() ) );
	m_timer->start( 10 );

	try
	{
		m_server = Server::Create( AuthPtr( new AuthToken ) );
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

int main( int argc, char** argv )
{
	QtApp app( argc, argv );
	return app.Run();
}
