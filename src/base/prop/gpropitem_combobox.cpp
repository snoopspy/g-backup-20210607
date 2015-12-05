#include "gpropitem_combobox.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemComboBox
// ----------------------------------------------------------------------------
GPropItemComboBox::GPropItemComboBox(GPropItemParam param) : GPropItem(param) {
  comboBox_ = new QComboBox(param.treeWidget_);
  param.treeWidget_->setItemWidget(item_, 1, comboBox_);
}

#endif // QT_GUI_LIB
