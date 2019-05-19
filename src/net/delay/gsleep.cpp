#include "gsleep.h"
#include <QThread>

// ----------------------------------------------------------------------------
// GSleep
// ----------------------------------------------------------------------------
void GSleep::sleep(GPacket* packet) {
  QThread::msleep(duration_);
  emit sleeped(packet);
}
