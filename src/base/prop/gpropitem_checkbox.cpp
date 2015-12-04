#include "gpropitem_checkbox.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemCheckBox
// ----------------------------------------------------------------------------
void GPropItemCheckBox::init(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, QObject* object, QMetaProperty mpro) {
  GPropItem::init(treeWidget, parentItem, object, mpro);
  checkBox_ = new QCheckBox(treeWidget);
  treeWidget->setItemWidget(item_, 1, checkBox_);
}

#endif // QT_GUI_LIB
