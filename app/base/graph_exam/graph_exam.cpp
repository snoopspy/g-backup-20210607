#include <QApplication>
#include <GApp>
#include <GGraphWidget>
#include <GJson>

#include "mygraph.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  GApp::init();

  GGraph graph;
  MyFactory myFactory(&graph);

  GGraphWidget graphWidget;
  graphWidget.setGraph(&graph);

  QJsonObject jo = GJson::loadFromFile();

  jo["graphWidget"] >> graphWidget;

  graphWidget.update();
  graphWidget.show();
  int res = a.exec();

  jo["graphWidget"] << graphWidget;

  GJson::saveToFile(jo);

  return res;
}
