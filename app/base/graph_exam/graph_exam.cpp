#include <QApplication>
#include <GApp>
#include <GJson>
#include <GPropWidget>

#include "mygraph.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  GApp::init();

  MyGraph myGraph;
  GPropWidget propWidget(&myGraph);

  QJsonObject jo = GJson::loadFromFile();

  jo["myGraph"] >> myGraph;

  propWidget.update();
  propWidget.show();
  int res = a.exec();

  jo["myGraph"] << myGraph;

  GJson::saveToFile(jo);

  return res;
}
