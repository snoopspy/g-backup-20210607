#include "gprop.h"
#include "base/gobj.h"

// ----------------------------------------------------------------------------
// GProp
// ----------------------------------------------------------------------------
void GProp::propLoad(QJsonObject jo) {
  QObject* object = dynamic_cast<QObject*>(this);
  if (object == nullptr) {
    qWarning() << "object is null";
    return;
  }
  const QMetaObject* mobj = object->metaObject();
  int count = mobj->propertyCount();
  for (int i = 0; i < count; i++) {
    QMetaProperty mpro = mobj->property(i);
    propLoad(jo, mpro);
  }
}

void GProp::propSave(QJsonObject& jo) {
  QObject* object = dynamic_cast<QObject*>(this);
  if (object == nullptr) {
    qWarning() << "object is null";
    return;
  }
  const QMetaObject* mobj = object->metaObject();
  int count = mobj->propertyCount();
  for (int i = 0; i < count; i++) {
    QMetaProperty mpro = mobj->property(i);
    propSave(jo, mpro);
  }
}

bool GProp::propLoad(QJsonObject jo, QMetaProperty mpro) {
  QObject* object = dynamic_cast<QObject*>(this);
  if (object == nullptr) {
    qWarning() << "object is null";
    return false;
  }
  const char* propName = mpro.name();
  int userType = mpro.userType();
  QVariant variant = ((const QJsonObject)jo)[propName];
  bool res = false;

  if (mpro.isEnumType()) {
    QMetaEnum menum = mpro.enumerator();
    QString key = variant.toString();
    if (key == "") return true;
    int value = menum.keyToValue(qPrintable(key));
    res = object->setProperty(propName, value);
  } else
  switch (userType) {
    case QMetaType::Bool:
      res = object->setProperty(propName, variant.toBool());
      break;

    case QMetaType::QChar:
      res = object->setProperty(propName, variant.toString());
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
        res = object->setProperty(propName, QVariant::fromValue<QString>(s));
        break;
      }

    case QMetaType::QString: {
        QString s = variant.toString();
        res = object->setProperty(propName, QVariant::fromValue<QString>(s));
        break;
      }
  }

  if (userType == qMetaTypeId<GObjPtr>()) {
    GObj* obj = qvariant_cast<GObj*>(object->property(propName));
    Q_ASSERT(obj != nullptr);
    obj->propLoad(jo[propName].toObject());
    return true;
  }

  if (userType == qMetaTypeId<GObjPtrListPtr>()) {
    GObjPtrListPtr list = qvariant_cast<GObjPtrListPtr>(object->property(propName));
    Q_ASSERT(list != nullptr);
    QJsonArray array = jo[propName].toArray();
    foreach (QJsonValue value, array) {
      QJsonObject childJo = value.toObject();
      GObj* childObj = list->addObj();
      childObj->propLoad(childJo);
    }
    return true;
  }

  if (!res) {
    qWarning() << QString("%1::propLoad(%2, %3) return false").arg(object->metaObject()->className(), propName, variant.toString());
  }
  return res;
}

bool GProp::propSave(QJsonObject& jo, QMetaProperty mpro) {
  QObject* object = dynamic_cast<QObject*>(this);
  if (object == nullptr) {
    qWarning() << "object is null";
    return false;
  }
  const char* propName = mpro.name();
  int userType = mpro.userType();
  QVariant variant = object->property(propName);

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
      jo[propName] = object->property(propName).toString();
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

  if (userType == qMetaTypeId<GObjPtr>()) {
    GObj* obj = qvariant_cast<GObj*>(variant);
    Q_ASSERT(obj != nullptr);
    QJsonObject childJo;
    obj->propSave(childJo);
    jo[propName] = childJo;
    return true;
  }

  if (userType == qMetaTypeId<GObjPtrListPtr>()) {
    GObjPtrListPtr list = qvariant_cast<GObjPtrListPtr>(variant);
    Q_ASSERT(list != nullptr);
    QJsonArray array;
    for (_GObjPtrList::iterator it = list->begin(); it != list->end(); it++) {
      QJsonObject childJo;
      GObj* childObj = *it;
      childObj->propSave(childJo);
      array.append(QJsonValue(childJo));
    }
    jo[propName] = array;
    return true;
  }

  qWarning() << QString("%1::propSave(%2, %3) return false").arg(object->metaObject()->className(), propName, variant.toString());
  return false;
}


#ifdef QT_GUI_LIB

#include "gpropitem_bool.h"
#include "gpropitem_char.h"
#include "gpropitem_enum.h"
#include "gpropitem_objptr.h"
#include "gpropitem_objptrlistptr.h"
#include "gpropitem_unknowntype.h"
#include "gpropitem_variant.h"

GPropItem* GProp::propCreateItem(GPropItemParam param) {
  const char* propName = param.mpro_.name();
  int userType = param.mpro_.userType();

  if (param.mpro_.isEnumType()) {
    return new GPropItemEnum(param);
  }

  switch (userType) {
    case QMetaType::Bool:
      return new GPropItemBool(param);

    case QMetaType::QChar:
      return new GPropItemChar(param);

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
      return new GPropItemVariant(param);
  }

  if (userType == qMetaTypeId<GObjPtr>())
    return new GPropItemObjPtr(param);

  if (userType == qMetaTypeId<GObjPtrListPtr>())
    return new GPropItemObjPtrListPtr(param);

  qWarning() << QString("can not create GPropItem(object=%1 propName=%2)").arg(param.object_->metaObject()->className(), propName);
  return nullptr;
}

void GProp::propCreateItems(QTreeWidget* treeWidget, QTreeWidgetItem* parent, QObject* object) {
  const QMetaObject* mobj = object->metaObject();
  int propCount = mobj->propertyCount();
  for (int i = 1; i < propCount; i++) { // skip objectName
    GPropItemParam param(treeWidget, parent, object, mobj->property(i));
    GPropItem* item = propCreateItem(param);
    if (item == nullptr) {
      qWarning() << QString("item is nullptr typeName='%1' name='%2'").arg(param.mpro_.typeName(), param.mpro_.name());
      item = new GPropItemUnknownType(GPropItemParam(param));
    }
    item->update();
  }
}

#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// GTEST
// ----------------------------------------------------------------------------
#ifdef GTEST
#include <gtest/gtest.h>

struct TestPerson : QObject, GProp {
  Q_OBJECT
  Q_PROPERTY(QString name MEMBER name_)
  Q_PROPERTY(int age MEMBER age_)

public:
  QString name_{""};
  int age_{0};
};

TEST(GProp, loadSaveTest) {
  TestPerson person;
  person.name_ = "gilgil";
  person.age_ = 20;

  QJsonObject jo;
  person.propSave(jo);

  TestPerson person2;
  person2.name_ = "marsaj";
  person2.age_ = 17;

  person2.propLoad(jo);

  EXPECT_EQ(person2.name_, "gilgil");
  EXPECT_EQ(person2.age_, 20);
}

#include "gprop.moc"

#endif // GTEST
