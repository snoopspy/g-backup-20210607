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

  GJson json;
  json.loadFromFile();
  QJsonObject& jo = json.jo_;

  jo["party"] >> party;
  jo["widget"] >> &widget;

  widget.update();
  widget.show();
  int res = a.exec();

  jo["party"] << party;
  jo["widget"] << &widget;
  json.saveToFile();

  return res;
}

