#include "base/gobj.h"
#include "gpropitem_objptr.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemObjPtr
// ----------------------------------------------------------------------------
void GPropItemObjPtr::init(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, QObject* object, QMetaProperty mpro) {
  GPropItem::init(treeWidget, parentItem, object, mpro);
  const char* propName = mpro.name();
  QVariant variant = object_->property(propName);
  GObj* childObj = qvariant_cast<GObj*>(variant);
  Q_ASSERT(childObj != nullptr);
  childObj->createPropItems(item_);
}

#endif // QT_GUI_LIB
