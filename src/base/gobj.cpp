#include <QJsonArray>
#include "gobj.h"

// ----------------------------------------------------------------------------
// GObj
// ----------------------------------------------------------------------------
QStringList GObj::methodList(QMetaMethod::MethodType methodType) {
  QStringList res;

  int _count = metaObject()->methodCount();
  for (int i = 0; i < _count; i++) {
    QMetaMethod _method = metaObject()->method(i);
    if (_method.access() != QMetaMethod::Public) continue;
    if (_method.methodType() != methodType) continue;
    if (_method.name().startsWith(("_"))) continue;
    QString methodName = _method.methodSignature();
    res.push_back(methodName);
  }
  return res;
}

QStringList GObj::signalList() {
  return methodList(QMetaMethod::Signal);
}

QStringList GObj::slotList() {
  return methodList(QMetaMethod::Slot);
}

bool GObj::connect(QObject* sender, const char* signal, QObject* receiver, const char* slot, Qt::ConnectionType type) {
  qDebug() << QString("connect %1::%2 > %3::%4 %5").
    arg(sender->metaObject()->className(), signal, receiver->metaObject()->className(), slot).arg(int(type));

  QByteArray newSignal;
  Q_ASSERT(signal != nullptr);
  if (*signal != '2') { // # define SIGNAL(a) qFlagLocation("2"#a QLOCATION)
    newSignal = "2" + QByteArray(signal);
    signal = newSignal;
  }

  QByteArray newSlot;
  Q_ASSERT(slot != nullptr);
  if (*slot != '1') { // # define SLOT(a) qFlagLocation("1"#a QLOCATION)
    newSlot = "1" + QByteArray(slot);
    slot = newSlot;
  }

  bool res = QObject::connect(sender, signal, receiver, slot, type);
  if (!res) {
    QString msg = QString("QObject::connect(%1::%2 > %3::%4 %5) return false").
      arg(sender->metaObject()->className(), signal, receiver->metaObject()->className(), slot).arg(int(type));
    qWarning() << msg;
    return false;
  }
  return res;
}

bool GObj::connect(QObject *sender, const QMetaMethod &signal, QObject *receiver, const QMetaMethod &slot, Qt::ConnectionType type) {
  QByteArray baSignal = signal.methodSignature();
  QByteArray baSlot  = slot.methodSignature();
  return GObj::connect(sender, baSignal.data(), receiver, baSlot.data(), type);
}

bool GObj::disconnect(QObject* sender, const char* signal, QObject* receiver, const char* slot) {
  qDebug() << QString("disconnect %1::%2 > %3::%4").
    arg(sender->metaObject()->className(), signal, receiver->metaObject()->className(), slot);

  QByteArray newSignal;
  Q_ASSERT(signal != nullptr);
  if (*signal != '2') { // # define SIGNAL(a) qFlagLocation("2"#a QLOCATION)
    newSignal = "2" + QByteArray(signal);
    signal = newSignal;
  }

  QByteArray newSlot;
  Q_ASSERT(slot != nullptr);
  if (*slot != '1') { // # define SLOT(a) qFlagLocation("1"#a QLOCATION)
    newSlot = "1" + QByteArray(slot);
    slot = newSlot;
  }

  bool res = QObject::disconnect(sender, signal, receiver, slot);
  if (!res) {
    QString msg = QString("QObject::disconnect(%1::%2 > %3::%4) return false").arg(
     sender->metaObject()->className(), signal, receiver->metaObject()->className(), slot);
   qWarning() << msg;
    return false;
  }
  return res;
}

bool GObj::disconnect(QObject *sender, const QMetaMethod &signal, QObject *receiver, const QMetaMethod &slot) {
  QByteArray baSignal = signal.methodSignature();
  QByteArray baSlot = slot.methodSignature();
  return GObj::disconnect(sender, baSignal.data(), receiver, baSlot.data());
}

QObject* GObj::createInstance(QString className) {
  if (!className.endsWith('*'))
    className += "*";
  int id = QMetaType::type(qPrintable(className));
  if (id == QMetaType::UnknownType) {
    qWarning() << QString("can not find class type for (%1)").arg(className);
    return nullptr;
  }
  const QMetaObject* mobj = QMetaType::metaObjectForType(id);
  Q_ASSERT(mobj != nullptr);
  QObject* obj = mobj->newInstance();
  if (obj == nullptr) {
    qWarning() << QString("newInstance failed for (%1)").arg(className);
    return nullptr;
  }
  return obj;
}
