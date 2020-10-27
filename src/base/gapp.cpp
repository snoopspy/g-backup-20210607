#include "gapp.h"
#include <QFile>
#include "base/graph/ggraphwidget.h"
#include "base/log/glogmanager.h"
#include "base/log/glogdbwin32.h"
#include "base/log/glogstderr.h"

// ----------------------------------------------------------------------------
// GApp
// ----------------------------------------------------------------------------
#ifdef QT_GUI_LIB
GApp::GApp(int &argc, char** argv) : QApplication(argc, argv) {
#else
GApp::GApp(int &argc, char** argv) : QCoreApplication(argc, argv) {
#endif // QT_GUI_LIB
	init();
}

void GApp::init() {
	GLogManager& logManager = GLogManager::instance();
	if (QFile::exists("sslog.ss")) {
		QJsonObject jo = GJson::loadFromFile("sslog.ss");
		QJsonArray ja = jo["logs"].toArray();
		logManager.load(ja);
	} else {
#ifdef Q_OS_WIN
	logManager.push_back(new GLogDbWin32);
#endif // Q_OS_WIN
#ifdef Q_OS_LINUX
	logManager.push_back(new GLogStderr);
#endif // Q_OS_LINUX
	}
}

#ifdef QT_GUI_LIB
bool GApp::execObj(GObj* obj) {
	GPropWidget propWidget(obj);

	QJsonObject jo = GJson::loadFromFile();
	jo["object"] >> *obj;
	jo["propWidget"] >> propWidget;

	propWidget.update();
	propWidget.show();
	int res = QApplication::exec();

	jo["object"] << *obj;
	jo["propWidget"] << propWidget;

	GJson::saveToFile(jo);
	return res;
}

bool GApp::execFactory(GPluginFactory* pluginFactory) {
	GGraph graph;
	return execGraphFactory(&graph, pluginFactory);
}

bool GApp::execGraphFactory(GGraph* graph, GPluginFactory* pluginFactory) {
	Q_ASSERT(graph != nullptr);

	if (pluginFactory == nullptr) {
		pluginFactory = &GPluginFactory::instance();
	}
	graph->setFactory(pluginFactory);

	GGraphWidget graphWidget;
	graphWidget.setGraph(graph);

	QJsonObject jo = GJson::loadFromFile();
	jo["graphWidget"] >> graphWidget;

	graphWidget.update();
	graphWidget.show();
	int res = QApplication::exec();

	jo["graphWidget"] << graphWidget;

	GJson::saveToFile(jo);
	return res;
}
#endif // QT_GUI_LIB
