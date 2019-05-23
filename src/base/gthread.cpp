#include "gthread.h"
#include <QDebug>

void GThread::start(Priority priority) {
  GThreadMgr& threadMgr = GThreadMgr::instance();
  if (threadMgr.suspended_) {
    GThreadMgr::ThreadInfo threadInfo;
    threadInfo.thread_ = this;
    threadInfo.priority_ = priority;
    threadMgr.threadInfos_.push_back(threadInfo);
  } else
    QThread::start(priority);
}

bool GThread::wait(unsigned long time) {
  bool res = QThread::wait(time);
  if (!res) {
    QString msg = QString("%1::wait(%2) return false").arg(metaObject()->className()).arg(time);
    QObject* _parent = parent();
    if (_parent != nullptr)
      msg += QString(" for (%1)").arg(_parent->metaObject()->className());
    qCritical() << "*********************************************************";
    qCritical() << msg;
    qCritical() << "*********************************************************";
    QThread::terminate();
  }
  return res;
}

// ----------------------------------------------------------------------------
// GThreadMgr
// ----------------------------------------------------------------------------
GThreadMgr::GThreadMgr() {
  qDebug() << "GThreadMgr::GThreadMgr()" << static_cast<void*>(this);
}

GThreadMgr::~GThreadMgr() {
  qDebug() << "GThreadMgr::~GThreadMgr()" << static_cast<void*>(this);
}

void GThreadMgr::suspendStart() {
  GThreadMgr& threadMgr = instance();
  threadMgr.suspended_ = true;
}

void GThreadMgr::resumeStart() {
  GThreadMgr& threadMgr = instance();
  for (ThreadInfo& threadInfo: threadMgr.threadInfos_) {
    threadInfo.thread_->start(threadInfo.priority_);
  }
  threadMgr.threadInfos_.clear();
  threadMgr.suspended_ = false;
}

GThreadMgr& GThreadMgr::instance() {
  static GThreadMgr threadMgr;
  return threadMgr;
}
