#include "gsleep.h"
#include <QThread>

// ----------------------------------------------------------------------------
// GSleep
// ----------------------------------------------------------------------------
bool GSleep::doOpen() {
  return true;
}

bool GSleep::doClose() {
  m_.lock();
  wc_.wakeOne();
  m_.unlock();
  return true;
}

void GSleep::sleep(GPacket* packet) {
  m_.lock();
  bool res = wc_.wait(&m_, duration_);
  m_.unlock();
  if (res == false) // time elapsed
    emit sleeped(packet);
}
