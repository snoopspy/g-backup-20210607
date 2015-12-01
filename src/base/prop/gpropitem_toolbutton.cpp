#include "gpropitem_toolbutton.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemToolButton
// ----------------------------------------------------------------------------
GPropItemToolButton::GPropItemToolButton(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro)
  : GPropItem(parent, object, mpro) {
  QTreeWidget* treeWidget = item_->treeWidget();
  Q_ASSERT(treeWidget != nullptr);
  toolButton_ = new QToolButton(treeWidget);
  toolButton_->setAutoRaise(true);
  treeWidget->setItemWidget(item_, 1, toolButton_);
}

#endif // QT_GUI_LIB
