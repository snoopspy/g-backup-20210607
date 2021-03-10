#include "widget.h"

#include <QApplication>
#include <GApp>

#ifdef GILGIL_ANDROID_DEBUG
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QThread>
#endif // GILGIL_ANDROID_DEBUG

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	GApp::init();
#ifdef GILGIL_ANDROID_DEBUG
	GApp::copyFileFromAssets("ssdemon",  QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);
	QString command = "su";
	QStringList arguments{"-c" ,QDir::currentPath() + "/ssdemon"};
	QProcess p;
	p.start(command, arguments);
	QThread::sleep(1);
#endif // GILGIL_ANDROID_DEBUG
	Widget w;
	w.show();
	return a.exec();
}
