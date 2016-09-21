#include "mygraph.h"

MyGraph::Factory::Factory(GGraph* graph) : GGraph::Factory(graph) {
  qRegisterMetaType<ObjA*>();
  qRegisterMetaType<ObjB*>();
  qRegisterMetaType<ObjC*>();

  ItemCategory* category = new ItemCategory("Obj");
  category->items_.push_back(new ItemNode(&ObjA::staticMetaObject));
  category->items_.push_back(new ItemNode(&ObjB::staticMetaObject));
  category->items_.push_back(new ItemNode(&ObjC::staticMetaObject));

  items_.push_back(category);
}
