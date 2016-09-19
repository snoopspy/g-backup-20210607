#include "mygraph.h"

MyGraph::Factory::Factory() {
  ItemCategory* category =new ItemCategory("Obj");
  category->items_.push_back(new ItemNode(&Obj1::staticMetaObject));
  category->items_.push_back(new ItemNode(&Obj2::staticMetaObject));
  category->items_.push_back(new ItemNode(&Obj3::staticMetaObject, "Hello. I am Obj3"));
  items_.push_back(category);
}
