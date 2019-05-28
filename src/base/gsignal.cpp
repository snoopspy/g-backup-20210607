#include "gsignal.h"
#include <QDebug>

// ----------------------------------------------------------------------------
// GSignal
// ----------------------------------------------------------------------------
GSignal::GSignal() {
}

GSignal::~GSignal() {
  for (Handlers::iterator it = handlers_.begin(); it != handlers_.end(); it++) {
    int signo = it.key();
    Handler handler = it.value();
    std::signal(signo, handler);
  }
}

void GSignal::signalHandler(int signo) {
  emit signaled(signo);

  Handlers::iterator it = handlers_.find(signo);
  if (it == handlers_.end()) {
    qCritical() << "can not find signal" << signo;
    return;
  }

  if (callOriginFunc_) {
    Handler handler = it.value();
    handler(signo);
  }

  if (signalOnce_) {
    Handler handler = it.value();
    std::signal(signo, handler);
    handlers_.erase(it);
  }
}

void GSignal::_signalHandler(int signo) {
  GSignal& signal = GSignal::instance();
  signal.signalHandler(signo);
}

void GSignal::setup(int signo) {
  Handler oldHandler = std::signal(signo, _signalHandler);
  handlers_[signo] = oldHandler;
}

GSignal& GSignal::instance() {
  static GSignal signal;
  return signal;
}
