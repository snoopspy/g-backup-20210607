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

void GSignal::_signalHandler(int signo) {
  GSignal& signal = GSignal::instance();
  emit signal.signaled(signo);

  Handlers::iterator it = signal.handlers_.find(signo);
  if (it == signal.handlers_.end()) {
    qCritical() << "can not find signal" << signo;
    return;
  }

  Handler handler = it.value();
  handler(signo);
}

void GSignal::setup(int signo) {
  Handler oldHandler = std::signal(signo, _signalHandler);
  handlers_[signo] = oldHandler;
}

GSignal& GSignal::instance() {
  static GSignal signal;
  return signal;
}
