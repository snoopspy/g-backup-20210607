#pragma once

#include <GGraph>

struct Obj : GObj {
//  Q_OBJECT
};

struct Obj1 : Obj {
  Q_OBJECT
};

struct Obj2 : Obj {
  Q_OBJECT
};

struct Obj3 : Obj {
  Q_OBJECT
};

struct MyGraph : GGraph {
  struct Factory : GGraph::Factory {
    Factory();
  };

  GGraph::Factory* factory() override {
    static Factory factory;
    return &factory;
  }
};
