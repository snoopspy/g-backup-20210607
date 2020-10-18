#include "gsscon.h"
#include <iostream>
#include <QFile>
#include "base/sys/gsignal.h"

using namespace std;

// ----------------------------------------------------------------------------
// GSsConParam
// ----------------------------------------------------------------------------
struct GSsConParam {
	QString fileName_;

	bool parse(QStringList arguments) {
		if (arguments.count() != 2) return false;
		fileName_ = arguments.at(1);
		return true;
	}

	static void usage() {
		using namespace std;
		QString appName = QCoreApplication::applicationName();
		cout << qPrintable(appName) << " " << G::version() << endl;
		cout << "Copyright (c) Gilbert Lee All rights reserved" << endl;
		cout << G::pcap_lib_version() << endl;
		cout << endl;
		cout << "usage : " << qPrintable(appName) << " <filename>" << endl;
		cout << endl;
		cout << "example : " << qPrintable(appName) << " ss/test.ss" << endl;
		cout << endl;
	}
};

// ----------------------------------------------------------------------------
// GSsCon
// ----------------------------------------------------------------------------
GSsCon::GSsCon(int &argc, char** argv) : GApp(argc, argv) {
}

GSsCon::~GSsCon() {
	graph_.close();
}

bool GSsCon::execFactory(GPluginFactory* pluginFactory) {
	GSsConParam param;
	if (!param.parse(arguments())) {
		GSsConParam::usage();
		return false;
	}

	if (!QFile::exists(param.fileName_)) {
		cout << "no such file " << qPrintable(param.fileName_) << endl;
		return false;
	}

	if (pluginFactory == nullptr) {
		pluginFactory = &GPluginFactory::instance();
	}
	graph_.setFactory(pluginFactory);

	GSignal& signal = GSignal::instance();
	signal.setup(SIGINT);
	QObject::connect(&signal, &GSignal::signaled, this, &GSsCon::processSignal);
	QObject::connect(&graph_, &GStateObj::closed, this, &GSsCon::processClose);

	QJsonObject jo = GJson::loadFromFile(param.fileName_);
	graph_.propLoad(jo);

	if (!graph_.open()) {
		qWarning() << graph_.err;
		return false;
	}

	bool res = QCoreApplication::exec();
	graph_.close();
	return res;
}

void GSsCon::processClose() {
	qDebug() << "processClose"; // gilgil temp 2016.09.25
	qDebug() << "bef call QCoreApplication::quit()"; // gilgil temp 2016.09.25
	QCoreApplication::quit();
	qDebug() << "aft call QCoreApplication::quit()"; // gilgil temp 2016.09.25
}

void GSsCon::processSignal(int signo) {
	qDebug() << "beg processSignal" << signo; // gilgil temp 2016.09.25
	if (signo == SIGINT) {
		qDebug() << "bef call close()" << signo; // gilgil temp 2016.09.25
		graph_.close();
		qDebug() << "aft call close()" << signo; // gilgil temp 2016.09.25
	}
	qDebug() << "end processSignal" << signo; // gilgil temp 2016.09.25
}
