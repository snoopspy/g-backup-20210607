#include "gdelay.h"
#include <QThread>

// ----------------------------------------------------------------------------
// GDelay
// ----------------------------------------------------------------------------
bool GDelay::doOpen() {
  return true;
}

bool GDelay::doClose() {
  we_.wakeAll();
  return true;
}

void GDelay::delay(GPacket* packet) {
  bool res = we_.wait(timeout_);
  if (res == false) // timeout elapsed
    emit delayed(packet);
}
