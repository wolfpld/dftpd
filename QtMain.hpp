#include <QObject>
#include <QtGui>
#include "Log.hpp"
#include "ServerPtr.hpp"

class QtApp : public QObject, public Log
{
	Q_OBJECT

public:
	QtApp( int argc, char** argv );
	~QtApp();

	int Run();
	void Print( const std::string& text );

private:
	QApplication* m_app;
	QTextEdit* m_logbox;

	ServerPtr m_server;

private slots:
	void TimerTick();
};
