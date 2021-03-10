#include <GApp>
#include "myfactory.h"

#ifdef GILGIL_ANDROID_DEBUG
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QThread>
#endif // GILGIL_ANDROID_DEBUG

int main(int argc, char *argv[]) {
	GApp a(argc, argv);
	MyFactory factory;
#ifdef GILGIL_ANDROID_DEBUG
	GApp::copyFileFromAssets("ssdemon",  QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner);
	QString command = "su";
	QStringList arguments{"-c" ,QDir::currentPath() + "/ssdemon"};
	QProcess p;
	p.start(command, arguments);
	QThread::sleep(1);
#endif // GILGIL_ANDROID_DEBUG
	return a.execFactory(&factory);
}
