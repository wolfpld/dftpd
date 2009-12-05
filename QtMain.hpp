#include <QTimer>
#include <QObject>
#include <QtGui>
#include "Log.hpp"
#include "ServerPtr.hpp"

class QtApp : public QMainWindow, public Log
{
	Q_OBJECT

public:
	QtApp( QApplication* app );
	~QtApp();

	int Run();
	void Print( const std::string& text );

private:
	void ServerCrashed();

	QApplication* m_app;
	QTextEdit* m_logbox;
	QTimer* m_timer;

	ServerPtr m_server;

private slots:
	void TimerTick();
};
