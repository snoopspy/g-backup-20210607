#include "base/gobj.h"
#include "gpropitem_objlist.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemObjList
// ----------------------------------------------------------------------------
GPropItemObjList::GPropItemObjList(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro)
  : GPropItemToolButton(parent, object, mpro) {
  const char* propName = mpro.name();
  QVariant variant = object_->property(propName);
  GObj* childObj = qvariant_cast<GObj*>(variant);
  Q_ASSERT(childObj != nullptr);
  childObj->createPropItems(item_);
}

#endif // QT_GUI_LIB
