#include "gtimestampdelay.h"
#include <QDeadlineTimer>
#include <QThread>

// ----------------------------------------------------------------------------
// GTimeStampDelay
// ----------------------------------------------------------------------------
bool GTimeStampDelay::doOpen() {
  et_.restart();
  lastClock_ = et_.elapsed();
  lastTs_ = 0;
  return true;
}

bool GTimeStampDelay::doClose() {
  m_.lock();
  wc_.wakeOne();
  m_.unlock();
  return true;
}

void GTimeStampDelay::delay(GPacket* packet) {
  qint64 nowTs = qint64(packet->ts_.tv_sec * 1000) + qint64(packet->ts_.tv_usec / 1000);
  if (lastTs_ == 0) lastTs_ = nowTs;
  qint64 remainTs = nowTs - lastTs_;
  qDebug() << "remainTs=" << remainTs << "nowTs=" << nowTs << "lastTs_=" << lastTs_; // gilgil temp 2019.05.20
  lastTs_ = nowTs;

  qint64 nowClock = et_.elapsed();
  qint64 remain = nowClock - lastClock_;
  qDebug() << "remain=" << remain << "now=" << nowClock << "last_=" << lastClock_; // gilgil temp 2019.05.20

  remainTs -= remain;
  if (remainTs < 0) {
    qCritical() << "remain is " << remain;
    remainTs = 0;
  }

  QDeadlineTimer dt(remainTs);
  m_.lock();
  bool res = wc_.wait(&m_, dt);
  m_.unlock();
  lastClock_ = et_.elapsed();
  if (res == false) // time elapsed
    emit delayed(packet);
}
