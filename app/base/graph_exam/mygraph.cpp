#include "mygraph.h"

MyGraph::Factory::Factory() {
  qRegisterMetaType<Obj1*>();
  qRegisterMetaType<Obj2*>();
  qRegisterMetaType<Obj3*>();

  ItemCategory* category = new ItemCategory("Obj");
  category->items_.push_back(new ItemNode(&Obj1::staticMetaObject));
  category->items_.push_back(new ItemNode(&Obj2::staticMetaObject));
  category->items_.push_back(new ItemNode(&Obj3::staticMetaObject));

  items_.push_back(category);
}
