#include "base/gobj.h"
#include "gpropitem_objptrvectorptr.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemObjPtrVectorPtr
// ----------------------------------------------------------------------------
GPropItemObjPtrVectorPtr::GPropItemObjPtrVectorPtr(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro)
  : GPropItemToolButton(parent, object, mpro) {
  toolButton_->setText("+");
  // gilgil temp 2015.12.02
}

#endif // QT_GUI_LIB
