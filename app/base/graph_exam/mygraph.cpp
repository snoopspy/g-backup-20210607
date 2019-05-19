#include "mygraph.h"

MyFactory::MyFactory(QObject* parent) : GPluginFactory(parent) {
  load("plugin");

  qRegisterMetaType<GMyObjA*>();
  qRegisterMetaType<GMyObjB*>();
  qRegisterMetaType<GMyObjC*>();

  ItemCategory* category = new ItemCategory("obj");
  category->items_.push_back(new ItemNode(&GMyObjA::staticMetaObject));
  category->items_.push_back(new ItemNode(&GMyObjB::staticMetaObject));
  category->items_.push_back(new ItemNode(&GMyObjC::staticMetaObject));

  items_.push_back(category);
}

MyFactory::~MyFactory() {
}
