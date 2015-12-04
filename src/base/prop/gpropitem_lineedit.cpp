#include "gpropitem_lineedit.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemLineEdit
// ----------------------------------------------------------------------------
void GPropItemLineEdit::init(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, QObject* object, QMetaProperty mpro) {
  GPropItem::init(treeWidget, parentItem, object, mpro);
  lineEdit_ = new QLineEdit(treeWidget);
  lineEdit_->setFrame(false);
  treeWidget->setItemWidget(item_, 1, lineEdit_);
}

#endif // QT_GUI_LIB
