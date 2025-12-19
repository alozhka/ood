#include "src/MainWindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	MainWindow mainWindow;
	mainWindow.resize(1000, 725);
	mainWindow.show();

	return QApplication::exec();
}
