#include "base/gobj.h"
#include "gpropitem_objptr.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemObjPtr
// ----------------------------------------------------------------------------
void GPropItemObjPtr::init(GPropItemParam param) {
  GPropItem::init(param);
  const char* propName = param.mpro_.name();
  QVariant variant = object_->property(propName);
  GObj* childObj = qvariant_cast<GObj*>(variant);
  Q_ASSERT(childObj != nullptr);
  childObj->createPropItems(item_);
}

#endif // QT_GUI_LIB
