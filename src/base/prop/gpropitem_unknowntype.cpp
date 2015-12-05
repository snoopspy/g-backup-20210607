#include "gpropitem_unknowntype.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemUnknownType
// ----------------------------------------------------------------------------
void GPropItemUnknownType::init(GPropItemParam param) {
  GPropItem::init(param);
  item_->setBackground(1, QBrush(Qt::lightGray));
}

#endif // QT_GUI_LIB
