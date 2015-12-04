#include "gpropitem_combobox.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemComboBox
// ----------------------------------------------------------------------------
void GPropItemComboBox::init(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, QObject* object, QMetaProperty mpro) {
  GPropItem::init(treeWidget, parentItem, object, mpro);
  comboBox_ = new QComboBox(treeWidget);
  treeWidget->setItemWidget(item_, 1, comboBox_);
}

#endif // QT_GUI_LIB
