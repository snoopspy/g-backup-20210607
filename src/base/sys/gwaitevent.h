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

#include <QDeadlineTimer>
#include <QWaitCondition>
#include <QMutex>

#include "base/gbase.h"

// ----------------------------------------------------------------------------
// GWaitEvent
// ----------------------------------------------------------------------------
struct G_EXPORT GWaitEvent {
  QWaitCondition wc_;
  QMutex m_;

  bool wait(GDuration timeout = ULONG_MAX) {
    m_.lock();
    bool res = wc_.wait(&m_, timeout);
    m_.unlock();
    return res;
  }

  bool wait(QDeadlineTimer& dt) {
    m_.lock();
    bool res = wc_.wait(&m_, dt);
    m_.unlock();
    return res;
  }

  void wakeOne() {
    m_.lock();
    wc_.wakeOne();
    m_.unlock();
  }

  void wakeAll() {
    m_.lock();
    wc_.wakeOne();
    m_.unlock();
  }
};
