#include "gpropitem_checkbox.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemCheckBox
// ----------------------------------------------------------------------------
void GPropItemCheckBox::init(GPropItemParam param) {
  GPropItem::init(param);
  checkBox_ = new QCheckBox(param.treeWidget_);
  param.treeWidget_->setItemWidget(item_, 1, checkBox_);
}

#endif // QT_GUI_LIB
