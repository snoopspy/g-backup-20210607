#include "base/gobj.h"
#include "gpropitem_objptr.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemObjPtr
// ----------------------------------------------------------------------------
GPropItemObjPtr::GPropItemObjPtr(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro)
  : GPropItem(parent, object, mpro) {
  const char* propName = mpro.name();
  QVariant variant = object_->property(propName);
  GObj* childObj = qvariant_cast<GObj*>(variant);
  Q_ASSERT(childObj != nullptr);
  childObj->createPropItems(item_);
}

#endif // QT_GUI_LIB
