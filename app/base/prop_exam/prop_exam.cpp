#include <QApplication>
#include <GApp>
#include <GJson>
#include <GPropWidget>

#include "party.h"
#include "person.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  GApp::init();

  QJsonObject jo = GJson::instance().loadFromFile();
  QJsonObject partyJo = jo["party"].toObject();
  QJsonObject widgetJo = jo["widget"].toObject();

  Party party;
  //Person party;
  party.load(partyJo);

  GPropWidget widget;
  widgetJo >> &widget;
  widget.setObject(&party);
  widget.show();
  int res = a.exec();

  party.save(partyJo);
  widgetJo << &widget;
  jo["party"] = partyJo;
  jo["widget"] = widgetJo;
  GJson::instance().saveToFile(jo);

  return res;
}

