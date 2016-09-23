#pragma once

#include <GGraph>

struct GMyObj : GStateObj {
  Q_OBJECT
  Q_PROPERTY(QString prop MEMBER prop_)

public:
  QString prop_;

public:
  GMyObj(QObject* parent = nullptr) : GStateObj(parent) {}

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

protected:
    bool doOpen() { emit signalMyObj(); return true; }
    bool doClose() { emit signalMyObjA(); return true; }

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

protected:
    bool doOpen() { emit signalMyObj(); return true; }
    bool doClose() { emit signalMyObjB(); return true; }

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

protected:
    bool doOpen() { emit signalMyObj(); return true; }
    bool doClose() { emit signalMyObjC(); return true; }

signals:
  void signalMyObjC();

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
