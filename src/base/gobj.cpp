#include "gobj.h"

// ----------------------------------------------------------------------------
// GObj
// ----------------------------------------------------------------------------
GObj::~GObj() {
}

void GObj::load(QJsonObject jo) {
  const QMetaObject* mobj = metaObject();
  int count = mobj->propertyCount();
  for (int i = 0; i < count; i++) {
    QMetaProperty mpro = mobj->property(i);
    load(jo, mpro);
  }
}

void GObj::save(QJsonObject& jo) {
  const QMetaObject* mobj = metaObject();
  int count = mobj->propertyCount();
  for (int i = 0; i < count; i++) {
    QMetaProperty mpro = mobj->property(i);
    save(jo, mpro);
  }
}

bool GObj::load(QJsonObject jo, QMetaProperty mpro) {
  const char* propName = mpro.name();
  int userType = mpro.userType();
  QVariant variant = ((const QJsonObject)jo)[propName];
  bool res = false;

  if (mpro.isEnumType()) {
    QMetaEnum menum = mpro.enumerator();
    QString key = variant.toString();
    if (key == "") return true;
    int value = menum.keyToValue(qPrintable(key));
    res = setProperty(propName, value);
  } else
  switch (userType) {
    case QMetaType::Bool:
      res = setProperty(propName, variant.toBool());
      break;

    case QMetaType::QChar:
      res = setProperty(propName, variant.toString());
      break;

    case QMetaType::Char:
    case QMetaType::Double:
    case QMetaType::Float:
    case QMetaType::Int:
    case QMetaType::LongLong:
    case QMetaType::Short:
    case QMetaType::UChar:
    case QMetaType::UInt:
    case QMetaType::ULongLong:
    case QMetaType::UShort: {
        QString s = variant.toString();
        if (s == "") return true;
        res = setProperty(propName, QVariant::fromValue<QString>(s));
        break;
      }

    case QMetaType::QString: {
        QString s = variant.toString();
        res = setProperty(propName, QVariant::fromValue<QString>(s));
        break;
      }
  }

  if (userType == qMetaTypeId<GObjRef>()) {
    GObj* obj = qvariant_cast<GObj*>(property(propName));
    Q_ASSERT(obj != nullptr);
    obj->load(jo[propName].toObject());
    return true;
  }

  if (!res) {
    qWarning() << QString("%1::load(%2, %3) return false").arg(metaObject()->className(), propName, variant.toString());
  }
  return res;
}

bool GObj::save(QJsonObject& jo, QMetaProperty mpro) {
  const char* propName = mpro.name();
  int userType = mpro.userType();
  QVariant variant = property(propName);

  if (mpro.isEnumType()) {
    QMetaEnum menum = mpro.enumerator();
    int value = variant.toInt();
    QString key = menum.valueToKey(value);
    jo[propName] = key;
    return true;
  }

  switch (userType) {
    case QMetaType::Bool:
      jo[propName] = variant.toBool();
      return true;

    case QMetaType::QChar:
      jo[propName] = property(propName).toString();
      return true;

    case QMetaType::Char:
    case QMetaType::Double:
    case QMetaType::Float:
    case QMetaType::Int:
    case QMetaType::LongLong:
    case QMetaType::Short:
    case QMetaType::UChar:
    case QMetaType::UInt:
    case QMetaType::ULongLong:
    case QMetaType::UShort: {
        QString s = variant.toString();
        jo[propName] = s;
        return true;
      }

    case QMetaType::QString: {
         QString s = variant.toString();
         jo[propName] = s;
         return true;
       }
  }

  if (userType == qMetaTypeId<GObjRef>()) {
    GObj* obj = qvariant_cast<GObj*>(variant);
    Q_ASSERT(obj != nullptr);
    QJsonObject childJson;
    obj->save(childJson);
    jo[propName] = childJson;
    return true;
  }

  qWarning() << QString("%1::save(%2, %3) return false").arg(metaObject()->className(), propName, variant.toString());
  return false;
}

#ifdef QT_GUI_LIB

#include "base/prop/gpropitem_bool.h"
#include "base/prop/gpropitem_enum.h"
#include "base/prop/gpropitem_objectname.h"
#include "base/prop/gpropitem_char.h"
#include "base/prop/gpropitem_objref.h"
#include "base/prop/gpropitem_variant.h"
#include "base/prop/gpropitem_unknowntype.h"

void GObj::createPropItems(QTreeWidgetItem* parent) {
  const QMetaObject* mobj = metaObject();
  int propCount = mobj->propertyCount();
  for (int i = 1; i < propCount; i++) { // skip objectName
    QMetaProperty mpro = mobj->property(i);
    GPropItem* item = createPropItem(parent, this, mpro);
    if (item == nullptr) {
      qWarning() << QString("item is nullptr typeName='%1' name='%2'").arg(mpro.typeName(), mpro.name());
      item = new GPropItemUnknownType(parent, this, mpro);
    }
    item->update();
  }
}

GPropItem* GObj::createPropItem(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro) {
  const char* propName = mpro.name();
  int userType = mpro.userType();

  if (mpro.isEnumType()) {
    return new GPropItemEnum(parent, object, mpro);
  }

  if ((QString)propName == "objectName") {
    return new GPropItemObjectName(parent, object, mpro);
  }

  switch (userType) {
    case QMetaType::Bool:
      return new GPropItemBool(parent, object, mpro);

    case QMetaType::QChar:
      return new GPropItemChar(parent, object, mpro);

    case QMetaType::Char:
    case QMetaType::Double:
    case QMetaType::Float:
    case QMetaType::Int:
    case QMetaType::LongLong:
    case QMetaType::QString:
    case QMetaType::Short:
    case QMetaType::UChar:
    case QMetaType::UInt:
    case QMetaType::ULongLong:
    case QMetaType::UShort:
      return new GPropItemVariant(parent, object, mpro);
  }

  if (userType == qMetaTypeId<GObjRef>())
    return new GPropItemObjRef(parent, object, mpro);

    qWarning() << QString("can not create GPropItem(object=%1 propName=%2)").arg(object->metaObject()->className(), propName);
  return nullptr;
}

#endif // QT_GUI_LIB
