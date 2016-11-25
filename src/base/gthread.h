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

#include <QThread>
#include "gbase.h"

// ----------------------------------------------------------------------------
// GThread
// ----------------------------------------------------------------------------
struct GThread : QThread {
  GThread(QObject *parent = nullptr) : QThread(parent) {}
  void start(Priority = InheritPriority);
  bool wait(unsigned long time = G_TIMEOUT);
};

// ----------------------------------------------------------------------------
// GThreadMgr
// ----------------------------------------------------------------------------
struct GThreadMgr {
  friend struct GThread;

  static void suspendStart();
  static void resumeStart();

protected:
  bool suspended_{false};

  struct GThreadInfo {
    GThread* thread_;
    QThread::Priority priority_;
  };
  QList<GThreadInfo> threadInfos_;
  static GThreadMgr& instance() {
    static GThreadMgr threadMgr;
    return threadMgr;
  }
};
