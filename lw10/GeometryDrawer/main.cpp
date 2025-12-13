#include "src/view/MainWindow.h"

#include <QApplication>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	MainWindow mainWindow;
	mainWindow.resize(1000, 700);
	mainWindow.show();

	return QApplication::exec();
}
