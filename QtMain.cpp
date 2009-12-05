#include "AuthNone.hpp"
#include "AuthToken.hpp"
#include "Exceptions.hpp"
#include "LogNull.hpp"
#include "QtMain.hpp"
#include "Server.hpp"

Log* g_log = NULL;

QtApp::QtApp( QApplication* app )
	: m_app( app )
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

void QtApp::About()
{
}

void QtApp::GenerateToken()
{
}

void QtApp::DisableAuthentication()
{
}

int main( int argc, char** argv )
{
	QtApp app( new QApplication( argc, argv ) );
	return app.Run();
}
