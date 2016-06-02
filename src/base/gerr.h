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

#include <errno.h>
//#include <ostream>
#include <QDebug>
#include <QSharedPointer>
#include <QString>

// ----------------------------------------------------------------------------
// GErr
// ----------------------------------------------------------------------------
struct GErr {
  enum {
    OK = 0,
    FAIL = -1,
    UNKNOWN = 1000,
    NOT_SUPPORTED,
    NOT_OPENED_STATE,
    NOT_CLOSED_STATE,
    VIRTUAL_FUNCTION_CALL,
    VALUE_IS_NULL,
    VALUE_IS_NOT_NULL,
    RETURN_NULL,
    RETURN_NOT_NULL,
  };

  enum {
    NET_CATEGORY = 2000
  };

  GErr() : code_(OK) {}
  GErr(int code) : code_(code) {}
  GErr(int code, QString msg) : code_(code), msg_(msg) {}
  virtual ~GErr() {}

  virtual const char* name() { return "Err"; }
  virtual int code() { return code_; }
  virtual QString msg() { return msg_; }

public: // protected: // gilgil temp 2016.06.02
  int code_;
  QString msg_;
};

QDebug operator << (QDebug os, GErr& err);
QDebug operator << (QDebug os, GErr&& err);
QDebug operator << (QDebug os, GErr* err);
QDebug operator << (QDebug os, QSharedPointer<GErr> err);

// ----------------------------------------------------------------------------
// GLastErr
// ----------------------------------------------------------------------------
struct GLastErr : public GErr {
  GLastErr() {
    code_ = errno;
    msg_ = strerror(code_);
  }

  const char* name() override { return "LastErr"; }
};
