#include <QtGui>
#include "AuthNone.hpp"
#include "AuthToken.hpp"
#include "Log.hpp"
#include "Server.hpp"
#include "ServerPtr.hpp"

Log* g_log = NULL;

class QtApp : public Log
{
public:
	QtApp( int argc, char** argv )
		: m_app( new QApplication( argc, argv ) )
	{
		g_log = this;
	}

	~QtApp()
	{
		delete m_app;
	}

	int Run()
	{
		QWidget window;
		window.show();
		window.setWindowTitle( "Dump FTP server" );

		m_logbox = new QTextEdit();
		m_logbox->setReadOnly( true );

		QVBoxLayout* layout = new QVBoxLayout();
		layout->addWidget( m_logbox );
		window.setLayout( layout );

		ServerPtr server( Server::Create( AuthPtr( new AuthToken ) ) );

		return m_app->exec();
	}

	void Print( const std::string& text )
	{
		m_logbox->append( QString( text.c_str() ) );
	}

private:
	QApplication* m_app;
	QTextEdit* m_logbox;
};

int main( int argc, char** argv )
{
	QtApp app( argc, argv );
	return app.Run();
}
