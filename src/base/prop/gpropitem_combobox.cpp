#include "gpropitem_combobox.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemComboBox
// ----------------------------------------------------------------------------
void GPropItemComboBox::init(GPropItemParam param) {
  GPropItem::init(param);
  comboBox_ = new QComboBox(param.treeWidget_);
  param.treeWidget_->setItemWidget(item_, 1, comboBox_);
}

#endif // QT_GUI_LIB
