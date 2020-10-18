#include "gapp.h"
#include "base/graph/ggraphwidget.h"
#include "net/gnetintf.h"
#include "net/grtm.h"
#include "base/log/glogmanager.h"
#include "base/log/glogdbwin32.h"
#include "base/log/glogfile.h"
#include "base/log/glogstdout.h"

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
#ifdef Q_OS_WIN
	logManager.logs_.push_back(new GLogDbWin32);
#endif // Q_OS_WIN
	logManager.logs_.push_back(new GLogFile);

	GNetIntf::all().init();
	GRtm::instance().init();
}

#ifdef QT_GUI_LIB
bool GApp::execObj(GObj* obj) {
	GPropWidget propWidget(obj);

	QJsonObject jo = GJson::loadFromFile();
	jo["obj"] >> *obj;
	jo["propWidget"] >> propWidget;

	propWidget.update();
	propWidget.show();
	int res = QApplication::exec();

	jo["obj"] << *obj;
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
