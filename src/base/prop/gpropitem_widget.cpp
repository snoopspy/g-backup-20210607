#include "gpropitem_widget.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemWidget
// ----------------------------------------------------------------------------
GPropItemWidget::GPropItemWidget(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro)
  : GPropItem(parent, object, mpro) {
  QTreeWidget* treeWidget = dynamic_cast<QTreeWidget*>(item_->treeWidget());
  Q_ASSERT(treeWidget != nullptr);
  widget_ = new QWidget(treeWidget);
  treeWidget->setItemWidget(item_, 1, widget_);
}

#endif // QT_GUI_LIB
