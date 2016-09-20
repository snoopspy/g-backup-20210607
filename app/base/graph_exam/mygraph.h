#pragma once

#include <GGraph>

struct Obj : GObj {
  Q_OBJECT

public:
  Obj(QObject* parent = nullptr) : GObj(parent) {}
};

struct Obj1 : Obj {
  Q_OBJECT
  Q_PROPERTY(QString name MEMBER name_)

public:
  Q_INVOKABLE Obj1(QObject* parent = nullptr) : Obj(parent) {
    qDebug() << "Obj1::Obj1";
  }
  ~Obj1() override {
    qDebug() << "Obj1::~Obj1";
  }
  QString name_;
};

struct Obj2 : Obj {
  Q_OBJECT

public:
  Q_INVOKABLE Obj2(QObject* parent = nullptr) : Obj(parent) {
    qDebug() << "Obj2::Obj2";
  }
  ~Obj2() override {
    qDebug() << "Obj2::~Obj2";
  }
};

struct Obj3 : Obj {
  Q_OBJECT

public:
  Q_INVOKABLE Obj3(QObject* parent = nullptr) : Obj(parent) {
    qDebug() << "Obj3::Obj3";
  }
  ~Obj3() override {
    qDebug() << "Obj3::~Obj3";
  }
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
