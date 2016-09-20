// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include <QObject>
#include <QSharedPointer>
#include "gbase.h"
#include "gerr.h"
#include "prop/gprop.h"

// ----------------------------------------------------------------------------
// GObj
// ----------------------------------------------------------------------------
struct GObj : QObject, GProp {
  Q_OBJECT

public:
  GObj(QObject* parent = nullptr) : QObject(parent) {}
  ~GObj() override {}

  QSharedPointer<GErr> err{nullptr};

public:
  static QObject* createInstance(QString className);
};

// ----------------------------------------------------------------------------
// GPtr
// ----------------------------------------------------------------------------
template <typename T>
struct GPtr {
  GPtr()              { p_ = nullptr; }
  GPtr(T* p)          { p_ = p;       }
  GPtr(T& t)          { p_ = &t;      }
  T* data()           { return p_;    }
  operator T*()       { return p_;    }
  operator T*() const { return p_;    }
  bool operator==(const T& other) { return p_ = other.p_; }
protected:
  T* p_;
};

// ----------------------------------------------------------------------------
// GObjPtr
// ----------------------------------------------------------------------------
struct GObjPtr : GPtr<GObj> {
  GObjPtr() : GPtr(nullptr) {}
  GObjPtr(GObj* obj) : GPtr(obj) {}
  GObjPtr(GObj& obj) : GPtr(obj) {}
};
//typedef GObj* GObjPtr;
Q_DECLARE_METATYPE(GObjPtr)

// ----------------------------------------------------------------------------
// GObjPtrListPtr
// ----------------------------------------------------------------------------
struct _GObjPtrList : QList<GObjPtr> {
  virtual GObj* addObj() = 0;
  virtual void delObj(GObj* obj) = 0;
};
typedef _GObjPtrList* GObjPtrListPtr;
Q_DECLARE_METATYPE(GObjPtrListPtr)

template <typename T>
struct GObjPtrList : _GObjPtrList {
  GObj* addObj() override {
    GObj* obj = new T;
    push_back(obj);
    return obj;
  }
  void delObj(GObj* obj) override {
    int index = indexOf(obj);
    if (index != -1)
      removeAt(index);
  }
};

// ----------------------------------------------------------------------------
// GObjPtrVectorPtr
// ----------------------------------------------------------------------------
typedef QVector<GObjPtr> GObjPtrVectorPtr;
Q_DECLARE_METATYPE(GObjPtrVectorPtr)

// ----------------------------------------------------------------------------
// SET_ERR
// ----------------------------------------------------------------------------
#define SET_ERR(CODE, MSG) { \
  if (err == nullptr) { \
    err = QSharedPointer<GErr>(new GErr((CODE), (MSG))); \
    qWarning() << err; \
  } \
}
