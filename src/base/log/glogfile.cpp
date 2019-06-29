#include "glogfile.h"
#include <QDate>

// ----------------------------------------------------------------------------
// GLogFile
// ----------------------------------------------------------------------------
GLogFile::GLogFile(QString folder) : GLog(nullptr), thread_(this) {
	folder_ = folder;
	if (!folder_.endsWith(QDir::separator()))
		folder_ += QDir::separator();
	QDir dir;
	if (!dir.mkpath(folder_)) {
		fprintf(stderr, "can not create directory(%s)\n", qPrintable(folder_));
	}
	changeFileName();
	thread_.start();
}

GLogFile::~GLogFile() {
	thread_.quit();
	thread_.wait();
}

void GLogFile::write(QString& msg) {
	QMutexLocker ml(&m_);
	file_.write(qPrintable(msg));
	file_.flush();
}

void GLogFile::changeFileName(){
	QDate now = QDate::currentDate();
	QString newFileName = now.toString("yyMMdd") + ".log";
	if (newFileName != fileName_) {
		QMutexLocker ml(&m_);
		fileName_ = newFileName;
		QString filePath = folder_ + fileName_;
		file_.close();
		file_.setFileName(filePath);
		if (!file_.open(QIODevice::Append | QIODevice::Text)) {
			fprintf(stderr, "error in file open(%s) error=%s\n", qPrintable(filePath), qPrintable(file_.errorString()));
		}
	}
}

void GLogFile::ChangeFileNameThread::quit(){
	we_.wakeAll();
	QThread::quit();
}

void GLogFile::ChangeFileNameThread::run() {
	while (true) {
		if (we_.wait(1000)) break; // signaled
		logFile_->changeFileName();
	}
}
