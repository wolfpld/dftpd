#include <QtGui>

int main( int argc, char** argv )
{
	QApplication app( argc, argv );

	QWidget window;
	window.show();
	window.setWindowTitle( "Dump FTP server" );

	QTextEdit* logBox = new QTextEdit();
	logBox->setReadOnly( true );

	QVBoxLayout* layout = new QVBoxLayout();
	layout->addWidget( logBox );
	window.setLayout( layout );

	return app.exec();
}
