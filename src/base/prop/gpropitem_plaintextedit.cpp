#include "gpropitem_plaintextedit.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemPlainTextEdit_
// ----------------------------------------------------------------------------
GPropItemPlainTextEdit_::GPropItemPlainTextEdit_(QWidget *parent, GPropItemPlainTextEdit* owner)
  : QPlainTextEdit(parent), owner_(owner) {
}

void GPropItemPlainTextEdit_::focusOutEvent(QFocusEvent * e) {
  Q_UNUSED(e)
  qDebug() << "focusOutEvent"; // gilgil temp 2016.04.06
  emit editingFinished();
}

// ----------------------------------------------------------------------------
// GPropItemPlainTextEdit
// ----------------------------------------------------------------------------
GPropItemPlainTextEdit::GPropItemPlainTextEdit(GPropItemParam param) : GPropItem(param) {
  plainTextEdit_ = new GPropItemPlainTextEdit_(param.treeWidget_, this);
  param.treeWidget_->setItemWidget(item_, 1, plainTextEdit_);
}

#endif // QT_GUI_LIB
