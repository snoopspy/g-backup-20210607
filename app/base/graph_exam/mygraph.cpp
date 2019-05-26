#include "mygraph.h"
#include <QCoreApplication>

MyFactory::MyFactory(QObject* parent) : GPluginFactory(parent) {
  load("plugin");

  qRegisterMetaType<GMyObjA*>();
  qRegisterMetaType<GMyObjB*>();
  qRegisterMetaType<GMyObjC*>();

  ItemCategory* category = new ItemCategory("obj");
  category->items_.push_back(new ItemNode(GMyObjA::staticMetaObject.className()));
  category->items_.push_back(new ItemNode(GMyObjB::staticMetaObject.className()));
  category->items_.push_back(new ItemNode(GMyObjC::staticMetaObject.className()));

  items_.push_back(category);
}

MyFactory::~MyFactory() {
}
