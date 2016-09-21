#pragma once

#include <GGraph>

struct Obj : GObj {
  Q_OBJECT

public:
  Obj(QObject* parent = nullptr) : GObj(parent) {}
};

struct ObjA : Obj {
  Q_OBJECT
  Q_PROPERTY(QString name MEMBER name_)

public:
  Q_INVOKABLE ObjA(QObject* parent = nullptr) : Obj(parent) {
    qDebug() << "ObjA::ObjA";
  }
  ~ObjA() override {
    qDebug() << "ObjA::~ObjA";
  }
  QString name_;
};

struct ObjB : Obj {
  Q_OBJECT

public:
  Q_INVOKABLE ObjB(QObject* parent = nullptr) : Obj(parent) {
    qDebug() << "ObjB::ObjB";
  }
  ~ObjB() override {
    qDebug() << "ObjB::~ObjB";
  }
};

struct ObjC : Obj {
  Q_OBJECT

public:
  Q_INVOKABLE ObjC(QObject* parent = nullptr) : Obj(parent) {
    qDebug() << "ObjC::ObjC";
  }
  ~ObjC() override {
    qDebug() << "ObjC::~ObjC";
  }
};
struct MyGraph : GGraph {
  struct Factory : GGraph::Factory {
    Factory(GGraph* graph);
  };

  GGraph::Factory* factory() override {
    static Factory factory(this);
    return &factory;
  }
};
