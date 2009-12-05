#include <QTimer>
#include "AuthNone.hpp"
#include "AuthToken.hpp"
#include "Server.hpp"
#include "QtMain.hpp"

Log* g_log = NULL;

QtApp::QtApp( int argc, char** argv )
	: m_app( new QApplication( argc, argv ) )
{
	g_log = this;
}

QtApp::~QtApp()
{
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

	m_server = Server::Create( AuthPtr( new AuthToken ) );

	QTimer* timer = new QTimer( this );
	connect( timer, SIGNAL( timeout() ), this, SLOT( TimerTick() ) );
	timer->start( 10 );

	return m_app->exec();
}

void QtApp::Print( const std::string& text )
{
	m_logbox->append( QString( text.c_str() ) );
}


void QtApp::TimerTick()
{
	m_server->Tick();
}

int main( int argc, char** argv )
{
	QtApp app( argc, argv );
	return app.Run();
}
