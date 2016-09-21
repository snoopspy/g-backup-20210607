#pragma once

#include <GGraph>

struct GMyObj : GObj {
  Q_OBJECT

public:
  GMyObj(QObject* parent = nullptr) : GObj(parent) {}
};

struct GMyObjA : GMyObj {
  Q_OBJECT
  Q_PROPERTY(QString name MEMBER name_)

public:
  Q_INVOKABLE GMyObjA(QObject* parent = nullptr) : GMyObj(parent) {
    qDebug() << "GMyObjA::GMyObjA";
  }
  ~GMyObjA() override {
    qDebug() << "GMyObjA::~GMyObjA";
  }
  QString name_;
};

struct GMyObjB : GMyObj {
  Q_OBJECT

public:
  Q_INVOKABLE GMyObjB(QObject* parent = nullptr) : GMyObj(parent) {
    qDebug() << "GMyObjB::GMyObjB";
  }
  ~GMyObjB() override {
    qDebug() << "GMyObjB::~GMyObjB";
  }
};

struct GMyObjC : GMyObj {
  Q_OBJECT

public:
  Q_INVOKABLE GMyObjC(QObject* parent = nullptr) : GMyObj(parent) {
    qDebug() << "GMyObjC::GMyObjC";
  }
  ~GMyObjC() override {
    qDebug() << "GMyObjC::~GMyObjC";
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
