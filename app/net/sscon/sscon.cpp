#include <iostream>
#include <GApp>
#include <GGraph>
#include <GSignal>
#include <GPluginFactory>

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
struct G_EXPORT GSsCon : QObject {
	Q_OBJECT

public:
	GSsCon(QObject* parent = nullptr) : QObject(parent) {}
	~GSsCon() override {
		graph_.close();
	}

	void prepareSignal() {
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
		signal.ignore(SIGPIPE); // Ignore SIGPIPE which can be signaled when TCP socket operation on linux
		signal.setup(SIGALRM);

		QObject::connect(&signal, &GSignal::signaled, this, &GSsCon::processSignal);
		QObject::connect(&graph_, &GStateObj::closed, this, &GSsCon::processClose);
	}

	int exec(GApp* a) {
		GSsConParam param;
		if (!param.parse(a->arguments())) {
			GSsConParam::usage();
			return false;
		}

		if (!QFile::exists(param.fileName_)) {
			cout << "no such file " << qPrintable(param.fileName_) << endl;
			return false;
		}

		GPluginFactory& pluginFactory = GPluginFactory::instance();
		graph_.setFactory(&pluginFactory);

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

protected:
	GGraph graph_;

public slots:
	void processClose() {
		QCoreApplication::quit();
	}

	void processSignal(int signo) {
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
};

int main(int argc, char* argv[]) {
	GApp a(argc, argv);
	GApp::launchDemon();
	GSsCon sc;
	sc.prepareSignal();
	return sc.exec(&a);
}

#include "sscon.moc"
