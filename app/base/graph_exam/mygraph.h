#pragma once

#include <GPluginFactory>

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
  bool doOpen() override { emit signalMyObj(); return true; }
  bool doClose() override { emit signalMyObjA(); return true; }

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
  bool doOpen() override { emit signalMyObj(); return true; }
  bool doClose() override { emit signalMyObjB(); return true; }

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
  bool doOpen() override { emit signalMyObj(); return true; }
  bool doClose() override { emit signalMyObjC(); return true; }

signals:
  void signalMyObjC();

public slots:
  void slotMyObjC() {
    qDebug() << "GMyObjC::slotMyObjC";
  }
};

struct MyFactory : GPluginFactory {
  MyFactory(QObject* parent = nullptr);
  ~MyFactory() override;
};
