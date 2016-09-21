#pragma once

#include <GGraph>

struct GMyObj : GObj {
  Q_OBJECT
  Q_PROPERTY(QString prop MEMBER prop_)

public:
  QString prop_;

public:
  GMyObj(QObject* parent = nullptr) : GObj(parent) {}

signals:
  void signalMyObj();

public slots:
  void slotMyObj() {
    qDebug() << "GMyObj::slotMyObj";
  }
};

struct GMyObjA : GMyObj {
  Q_OBJECT
  Q_PROPERTY(QString propA MEMBER propA_)

public:
    QString propA_;

public:
  Q_INVOKABLE GMyObjA(QObject* parent = nullptr) : GMyObj(parent) {
    qDebug() << "GMyObjA::GMyObjA";
  }
  ~GMyObjA() override {
    qDebug() << "GMyObjA::~GMyObjA";
  }

signals:
  void signalMyObjA();

public slots:
  void slotMyObjA() {
    qDebug() << "GMyObjA::slotMyObjA";
  }
};

struct GMyObjB : GMyObj {
  Q_OBJECT
  Q_PROPERTY(QString propB MEMBER propB_)

public:
    QString propB_;

public:
  Q_INVOKABLE GMyObjB(QObject* parent = nullptr) : GMyObj(parent) {
    qDebug() << "GMyObjB::GMyObjB";
  }
  ~GMyObjB() override {
    qDebug() << "GMyObjB::~GMyObjB";
  }
  QString nameA_;

signals:
  void signalMyObjB();

public slots:
  void slotMyObjB() {
    qDebug() << "GMyObjB::slotMyObjB";
  }
};

struct GMyObjC : GMyObj {
  Q_OBJECT
  Q_PROPERTY(QString propB MEMBER propB_)

public:
    QString propB_;

public:
  Q_INVOKABLE GMyObjC(QObject* parent = nullptr) : GMyObj(parent) {
    qDebug() << "GMyObjC::GMyObjC";
  }
  ~GMyObjC() override {
    qDebug() << "GMyObjC::~GMyObjC";
  }

signals:
  void signalMyObjB();

public slots:
  void slotMyObjC() {
    qDebug() << "GMyObjC::slotMyObjC";
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
