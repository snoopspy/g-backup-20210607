#include <QJsonArray>
#include "gobj.h"

// ----------------------------------------------------------------------------
// GObj
// ----------------------------------------------------------------------------
QStringList GObj::methodList(QMetaMethod::MethodType methodType) {
  QStringList res;

  int _count = metaObject()->methodCount();
  for (int i = 0; i < _count; i++)
  {
    QMetaMethod _method = metaObject()->method(i);
    if (_method.access() == QMetaMethod::Public && _method.methodType() == methodType)
    {
      QString methodName = _method.methodSignature();
      res.push_back(methodName);
     }
  }
  return res;
}

QStringList GObj::signalList() {
  return methodList(QMetaMethod::Signal);
}

QStringList GObj::slotList() {
  return methodList(QMetaMethod::Slot);
}

QObject* GObj::createInstance(QString className) {
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
    qWarning() << QString("can not create class for (%1)").arg(className);
    return nullptr;
  }
  return obj;
}
