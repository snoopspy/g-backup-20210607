#include "gpropitem_toolbutton.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemToolButton
// ----------------------------------------------------------------------------
void GPropItemToolButton::init(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, QObject* object, QMetaProperty mpro) {
  GPropItem::init(treeWidget, parentItem, object, mpro);
  toolButton_ = new QToolButton(treeWidget);
  toolButton_->setAutoRaise(true);
  treeWidget->setItemWidget(item_, 1, toolButton_);
}

#endif // QT_GUI_LIB
