#include "gdelay.h"
#include <QDeadlineTimer>
#include <QThread>

// ----------------------------------------------------------------------------
// GDelay
// ----------------------------------------------------------------------------
bool GDelay::doOpen() {
  et_.restart();
  last_ = et_.elapsed();
  return true;
}

bool GDelay::doClose() {
  m_.lock();
  wc_.wakeOne();
  m_.unlock();
  return true;
}

void GDelay::delay(GPacket* packet) {
  qint64 now = et_.elapsed();
  qint64 remain = qint64(duration_) - (now - last_);
  qDebug() << "remain=" << remain << "now=" << now << "last_=" << last_; // gilgil temp 2019.05.19
  if (remain < 0) {
    // qDebug() << "remain is " << remain;
    remain = 0;
  }
  QDeadlineTimer dt(remain);
  m_.lock();
  bool res = wc_.wait(&m_, dt);
  m_.unlock();
  last_ = et_.elapsed();
  if (res == false) // time elapsed
    emit delayed(packet);
}
