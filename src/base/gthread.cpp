#include "gthread.h"
#include <QDebug>

// ----------------------------------------------------------------------------
// GThread
// ----------------------------------------------------------------------------
bool GThread::wait(unsigned long time) {
  bool res = QThread::wait(time);
  if (!res) {
    QString msg = QString("%1::wait(%2) return false").arg(metaObject()->className()).arg(time);
    QObject* _parent = parent();
    if (_parent != nullptr)
      msg += QString(" for (%1)").arg(_parent->metaObject()->className());
    qCritical() << "*********************************************************";
    qCritical() << msg;
    qCritical() << "*********************************************************";
    QThread::terminate();
  }
  return res;
}
