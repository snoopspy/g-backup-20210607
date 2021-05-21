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
	signal.setup(SIGILL);
	signal.setup(SIGABRT);
	signal.setup(SIGFPE);
	signal.setup(SIGSEGV);
	signal.setup(SIGTERM);
	signal.setup(SIGHUP);
	signal.setup(SIGQUIT);
	signal.setup(SIGTRAP);
	signal.setup(SIGKILL);
	signal.setup(SIGBUS);
	signal.setup(SIGSYS);
	// ----- by gilgil 2021.03.13 -----
	// Ignore SIGPIPE which can be signaled when TCP socket operation on linux
	// std::signal(SIGPIPE, signalHandler);
	signal.ignore(SIGPIPE);
	// --------------------------------
	signal.setup(SIGALRM);



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
	QCoreApplication::quit();
}

void GSsCon::processSignal(int signo) {
	QString signal = "unknown";
	switch (signo) {
		case SIGINT: signal = "SIGINT"; break;
		case SIGILL: signal = "SIGILL"; break;
		case SIGABRT: signal = "SIGABRT"; break;
		case SIGFPE: signal = "SIGFPE"; break;
		case SIGSEGV: signal = "SIGSEGV"; break;
		case SIGTERM: signal = "SIGTERM"; break;
		case SIGHUP: signal = "SIGHUP"; break;
		case SIGQUIT: signal = "SIGQUIT"; break;
		case SIGTRAP: signal = "SIGTRAP"; break;
		case SIGKILL: signal = "SIGKILL"; break;
		case SIGBUS: signal = "SIGBUS"; break;
		case SIGSYS: signal = "SIGSYS"; break;
		case SIGPIPE: signal = "SIGPIPE"; break;
		case SIGALRM: signal = "SIGALRM"; break;
	}
	QString msg = strsignal(signo);
	qWarning() << QString("signo=%1 signal=%2 msg=%3").arg(signo).arg(signal, msg);

	graph_.close();
}
