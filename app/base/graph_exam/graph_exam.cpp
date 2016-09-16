#include <QApplication>
#include <GApp>
#include <GJson>
#include <GPropWidget>

#include "dept.h"
#include "person.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  GApp::init();

  Dept dept;
  GPropWidget propWidget(&dept);

  QJsonObject jo = GJson::loadFromFile();

  jo["dept"] >> dept;
  jo["propWidget"] >> propWidget;

  propWidget.update();
  propWidget.show();
  int res = a.exec();

  jo["dept"] << dept;
  jo["propWidget"] << propWidget;

  GJson::saveToFile(jo);

  return res;
}
