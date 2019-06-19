#include "glogmanager.h"
#include <QDateTime>
#include <QDir>
#include <QThread>

// ----------------------------------------------------------------------------
// qt_gettid(coiped from qlogging.cpp)
// ----------------------------------------------------------------------------
#ifdef Q_OS_LINUX
#include <unistd.h>
#include <sys/syscall.h>
static long qt_gettid()
{
		return syscall(SYS_gettid);
}
#endif
#ifdef Q_OS_WIN
static QT_PREPEND_NAMESPACE(qint64) qt_gettid()
{
		QT_USE_NAMESPACE
		return qintptr(QThread::currentThreadId());
}
#endif

// ----------------------------------------------------------------------------
// GLogManager
// ----------------------------------------------------------------------------
GLogManager::GLogManager(QObject* parent) : GObj(parent) {
	qInstallMessageHandler(myMessageOutput);
}

GLogManager::~GLogManager() {
	for (GLog* log: logs_) {
		delete log;
	}
}

GLogManager& GLogManager::instance() {
	static GLogManager logManager;
	return logManager;
}

void GLogManager::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
	QString typeStr;
	switch (type) {
		case QtDebugMsg: typeStr = "D"; break;
		case QtWarningMsg: typeStr = "W"; break;
		case QtCriticalMsg: typeStr = "C"; break;
		case QtFatalMsg: typeStr = "F"; break;
		case QtInfoMsg: typeStr = "I"; break;
	}
	QDateTime now = QDateTime::currentDateTime();
	QString nowStr = now.toString("yyMMdd hh:mm:ss.zzz");
	QString threadStr = QString::number(qt_gettid());
	QString fileStr = context.file;
	int i = fileStr.lastIndexOf(QDir::separator());
	if (i != -1) fileStr = fileStr.mid(i + 1);
	QString lineStr = QString::number(context.line);
	QString funcStr = context.function;
	i = funcStr.lastIndexOf(' ');
	if (i != -1) funcStr = funcStr.mid(i + 1);
	QString finalMsg = QString("%1 %2 %3 [%4:%5 %6] %7").arg(typeStr, nowStr, threadStr, fileStr, lineStr, funcStr, msg);

	GLogManager& logManager = GLogManager::instance();
	for (GLog* log: logManager.logs_)
		log->write(finalMsg);
}
