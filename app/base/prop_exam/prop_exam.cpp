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
  GPropWidget widget(&dept);

  QJsonObject jo = GJson::loadFromFile();

  jo["dept"] >> dept;
  jo["rect"] >> GJson::rect(&widget);
  jo["sizes"] >> GJson::headerSizes(&widget);

  widget.update();
  widget.show();
  int res = a.exec();

  jo["dept"] << dept;
  jo["rect"] << GJson::rect(&widget);
  jo["sizes"] << GJson::headerSizes(&widget);

  GJson::saveToFile(jo);

  return res;
}
