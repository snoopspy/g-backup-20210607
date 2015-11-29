#include <QApplication>
#include <GApp>
#include <GJson>
#include <GPropWidget>

#include "party.h"
#include "person.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  GApp::init();

  Party party;
  GPropWidget widget(&party);

  QJsonObject jo = GJson::loadFromFile();

  jo["party"] >> party;
  jo["rect"] >> GJson::rect(&widget);
  jo["sizes"] >> GJson::headerSizes(&widget);

  widget.update();
  widget.show();
  int res = a.exec();

  jo["party"] << party;
  jo["rect"] << GJson::rect(&widget);
  jo["sizes"] << GJson::headerSizes(&widget);

  GJson::saveToFile(jo);

  return res;
}
