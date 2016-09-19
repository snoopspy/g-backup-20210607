#include <QApplication>
#include <GApp>
#include <GGraphWidget>
#include <GJson>

#include "mygraph.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  GApp::init();

  MyGraph myGraph;
  GGraphWidget graphWidget(&myGraph);

  QJsonObject jo = GJson::loadFromFile();

  jo["myGraph"] >> myGraph;
  jo["graphWidget"] >> graphWidget;

  graphWidget.update();
  graphWidget.show();
  int res = a.exec();

  jo["myGraph"] << myGraph;
  jo["graphWidget"] << graphWidget;

  GJson::saveToFile(jo);

  return res;
}
