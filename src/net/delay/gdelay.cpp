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

void GDelay::sleep(GPacket* packet) {
  bool res = we_.wait(duration_);
  if (res == false) // time elapsed
    emit sleeped(packet);
}
