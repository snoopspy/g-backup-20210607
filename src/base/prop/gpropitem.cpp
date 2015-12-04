#include "gpropitem.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItem
// ----------------------------------------------------------------------------
void GPropItem::init(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, QObject* object, QMetaProperty mpro) {
  setParent(treeWidget);
  if (parentItem == nullptr)
    item_ = new QTreeWidgetItem(treeWidget);
  else
    item_ = new QTreeWidgetItem(parentItem);
  object_ = object;
  mpro_ = mpro;
  item_->setText(0, mpro_.name());
  if (parentItem == nullptr)
    treeWidget->addTopLevelItem(item_);
}

#endif // QT_GUI_LIB
