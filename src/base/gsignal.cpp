#include "gsignal.h"
#include <QDebug>
#include "base/gerr.h"

// ----------------------------------------------------------------------------
// GSignal
// ----------------------------------------------------------------------------
GSignal::GSignal() {
}

GSignal::~GSignal() {
  for (Handlers::iterator it = handlers_.begin(); it != handlers_.end(); it++) {
    Handler* handler = it.value();
    delete handler;
  }
}

void GSignal::_sigFunc(int signo) {
  GSignal* signal = instance();
  emit signal->signaled(signo);

  Handlers::iterator it = signal->handlers_.find(signo);
  if (it == signal->handlers_.end()) {
    qCritical() << "can not find signal" << signo;
    return;
  }

  Handler* handler = it.value();
  int res = sigaction(signo, &handler->oldAction_, nullptr);
  if (res != 0) {
    qCritical() << QString("sigaction(%1) return %2").arg(signo).arg(res) << GLastErr();
  }
}

bool GSignal::setup(int signo) {
  Handler* handler = new Handler;
  handler->myAction_.sa_handler = _sigFunc;
  int res = sigaction(signo, &handler->myAction_, &handler->oldAction_);
  if (res != 0) {
    qCritical() << QString("sigaction(%1) return %2").arg(signo).arg(res) << GLastErr();
    delete handler;
    return false;
  }
  handlers_[signo] = handler;
  return true;
}

GSignal* GSignal::instance() {
  static GSignal self;
  return &self;
}

