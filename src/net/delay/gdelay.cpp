#include "gdelay.h"
#include <QDeadlineTimer>
#include <QThread>

// ----------------------------------------------------------------------------
// GDelay
// ----------------------------------------------------------------------------
bool GDelay::doOpen() {
  et_.restart();
  lastClock_ = et_.elapsed();
  return true;
}

bool GDelay::doClose() {
  we_.wakeAll();
  return true;
}

void GDelay::delay(GPacket* packet) {
  qint64 nowClock = et_.elapsed();
  qint64 remain = qint64(duration_) - (nowClock - lastClock_);
  qDebug() << "remain=" << remain << "nowClock=" << nowClock << "lastClock_=" << lastClock_; // gilgil temp 2019.05.19

  if (remain < 0) {
    qCritical() << "remain is " << remain;
    remain = 0;
  }

  QDeadlineTimer dt(remain);
  bool res = we_.wait(dt);
  lastClock_ = et_.elapsed();
  if (res == false) // time elapsed
    emit delayed(packet);
}
