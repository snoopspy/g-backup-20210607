#include "base/gobj.h"
#include "gpropitem_objvector.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemObjVector
// ----------------------------------------------------------------------------
GPropItemObjVector::GPropItemObjVector(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro)
  : GPropItemToolButton(parent, object, mpro) {
  toolButton_->setText("+");
}

#endif // QT_GUI_LIB
