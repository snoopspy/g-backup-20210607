#include "base/gobj.h"
#include "gpropitem_objptr.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemObjPtr
// ----------------------------------------------------------------------------
GPropItemObjPtr::GPropItemObjPtr(GPropItemParam param) : GPropItem(param) {
  const char* propName = param.mpro_.name();
  QVariant variant = object_->property(propName);
  GObj* childObj = qvariant_cast<GObj*>(variant);
  Q_ASSERT(childObj != nullptr);
  childObj->propCreateItems(param.treeWidget_, item_, childObj);
}

#endif // QT_GUI_LIB
