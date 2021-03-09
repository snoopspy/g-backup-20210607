#include "widget.h"

#include <QApplication>
#include <GApp>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	GApp::init();
	Widget w;
	w.show();
	return a.exec();
}
