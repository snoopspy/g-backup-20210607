#include <QJsonArray>
#include "gobj.h"

// ----------------------------------------------------------------------------
// GObj
// ----------------------------------------------------------------------------
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
