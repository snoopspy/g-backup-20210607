#include "gpropitem_widget.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemWidget
// ----------------------------------------------------------------------------
void GPropItemWidget::init(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, QObject* object, QMetaProperty mpro) {
  GPropItem::init(treeWidget, parentItem, object, mpro);
  widget_ = new QWidget(treeWidget);
  treeWidget->setItemWidget(item_, 1, widget_);
}

#endif // QT_GUI_LIB
