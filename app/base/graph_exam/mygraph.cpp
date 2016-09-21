#include "mygraph.h"

MyGraph::Factory::Factory() {
  qRegisterMetaType<GMyObjA*>();
  qRegisterMetaType<GMyObjB*>();
  qRegisterMetaType<GMyObjC*>();

  ItemCategory* category = new ItemCategory("Obj");
  category->items_.push_back(new ItemNode(&GMyObjA::staticMetaObject));
  category->items_.push_back(new ItemNode(&GMyObjB::staticMetaObject));
  category->items_.push_back(new ItemNode(&GMyObjC::staticMetaObject));

  items_.push_back(category);
}
