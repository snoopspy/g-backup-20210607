#include "base/gobj.h"
#include "gpropitem_objlist.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemObjList
// ----------------------------------------------------------------------------
GPropItemObjList::GPropItemObjList(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro)
  : GPropItemToolButton(parent, object, mpro) {
  toolButton_->setText("+");
  QObject::connect(toolButton_, SIGNAL(clicked(bool)), this, SLOT(myClicked(bool)));
}

void GPropItemObjList::myClicked(bool checked) {
  qDebug() << ""; // gilgil temp 2015.12.02
}

#endif // QT_GUI_LIB
