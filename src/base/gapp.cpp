#include "gapp.h"
#include <QString>
#include <GGraph>
#include <GGraphWidget>

// ----------------------------------------------------------------------------
// GApp
// ----------------------------------------------------------------------------
#ifdef QT_GUI_LIB
GApp::GApp(int &argc, char **argv) : QApplication (argc, argv) {
#else
GApp::GApp(int &argc, char **argv) : QCoreApplication (argc, argv) {
#endif // QT_GUI_LIB
  init();
}

void GApp::init() {
  qSetMessagePattern("%{time yyyyMMdd h:mm:ss.zzz} %{threadid} [%{file}:%{line}] %{message}");
}

#ifdef QT_GUI_LIB
bool GApp::exec_(GObj* obj) {
  GPropWidget propWidget(obj);

  QJsonObject jo = GJson::loadFromFile();

  jo["dept"] >> *obj;
  jo["propWidget"] >> propWidget;

  propWidget.update();
  propWidget.show();
  int res = QApplication::exec();

  jo["dept"] << *obj;
  jo["propWidget"] << propWidget;

  GJson::saveToFile(jo);

  return res;
}

bool GApp::exec_(GGraph* graph, GPluginFactory* pluginFactory) {
  bool deleteGraph = false;
  if (graph == nullptr) {
    graph = new GGraph;
    deleteGraph = true;
  }

  if (pluginFactory == nullptr) {
    pluginFactory = &GPluginFactory::instance();
    pluginFactory->load("plugin");
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

  if (deleteGraph) delete graph;

  return res;
}
#endif // QT_GUI_LIB
