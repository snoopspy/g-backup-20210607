// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include "glog.h"
#include "base/sys/gwaitevent.h"
#include <QFile>
#include <QThread>

// ----------------------------------------------------------------------------
// GLogFile
// ----------------------------------------------------------------------------
struct G_EXPORT GLogFile : GLog {
	GLogFile(QString folder = "log");
	~GLogFile() override;
	void write(QString& msg) override;

protected:
	QString folder_;
	QMutex m_;
	QString fileName_;
	QFile file_;
	void changeFileName();

	struct ChangeFileNameThread : QThread {
		ChangeFileNameThread(GLogFile* logFile) : logFile_(logFile) {}
		GLogFile* logFile_;
		GWaitEvent we_;
		void quit();
		void run() override;
	} thread_;
};
