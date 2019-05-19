#include "gsleep.h"
#include <QThread>

// ----------------------------------------------------------------------------
// GSleep
// ----------------------------------------------------------------------------
bool GSleep::doOpen() {
  return true;
}

bool GSleep::doClose() {
  we_.wakeAll();
  return true;
}

void GSleep::sleep(GPacket* packet) {
  bool res = we_.wait(duration_);
  if (res == false) // time elapsed
    emit sleeped(packet);
}
