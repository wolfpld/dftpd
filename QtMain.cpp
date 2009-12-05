#include <QtGui>

int main( int argc, char** argv )
{
	QApplication app( argc, argv );

	QWidget window;
	window.show();
	window.setWindowTitle( "Dump FTP server" );

	return app.exec();
}
