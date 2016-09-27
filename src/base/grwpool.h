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

#include <QMutex>
#include <QWaitCondition>
#include "gstateobj.h"

// ----------------------------------------------------------------------------
// GRwPool
// ----------------------------------------------------------------------------
template <typename T>
struct GRwPool : GStateObj {
  GRwPool(QObject* parent = nullptr) : GStateObj(parent) {
  }

  ~GRwPool() {
    close();
    if (items_ != nullptr) {
      delete[] items_;
      items_ = nullptr;
    }
  }

  void setCount(int count) {
    count_ = count;
  }

  T read() {
    if (!active())
      return (T)0;

    mutex_.lock();
    if (index_ == 0)
      notEmpty_.wait(&mutex_);
    mutex_.unlock();

    if (!active())
      return (T)0;

    mutex_.lock();
    Q_ASSERT(index_ > 0);
    T res = items_[--index_];
    notFull_.wakeAll();
    mutex_.unlock();

    return res;
  }

  void write(T t) {
    if (!active())
      return;

    mutex_.lock();
    if (index_ + 1 >= count_)
      notFull_.wait(&mutex_);
    mutex_.unlock();

    if (!active())
      return;

    mutex_.lock();
    Q_ASSERT(index_ < count_);
    items_[index_++] = t;
    notEmpty_.wakeAll();
    mutex_.unlock();
  }

protected:
  bool doOpen() override {
    if (count_ == 0) {
      SET_ERR(GErr::VALUE_IS_ZERO, "count_ is zero");
      return false;
    }
    if (items_ != nullptr)
      delete[] items_;
    items_ = new T[count_];
    return true;
  }

  bool doClose() override {
    notEmpty_.wakeAll();
    notFull_.wakeAll();
    return true;
  }

protected:
  size_t count_{0};
  size_t index_{0};
  T* items_{nullptr};

  QWaitCondition notEmpty_;
  QWaitCondition notFull_;
  QMutex mutex_;
};
