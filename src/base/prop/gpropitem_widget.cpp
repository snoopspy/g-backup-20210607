#include "gpropitem_widget.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemWidget
// ----------------------------------------------------------------------------
void GPropItemWidget::init(GPropItemParam param) {
  GPropItem::init(param);
  widget_ = new QWidget(param.treeWidget_);
  param.treeWidget_->setItemWidget(item_, 1, widget_);
}

#endif // QT_GUI_LIB
