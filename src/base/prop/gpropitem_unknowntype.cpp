#include "gpropitem_unknowntype.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemUnknownType
// ----------------------------------------------------------------------------
void GPropItemUnknownType::init(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, QObject* object, QMetaProperty mpro) {
  GPropItem::init(treeWidget, parentItem, object, mpro);
  item_->setBackground(1, QBrush(Qt::lightGray));
}

#endif // QT_GUI_LIB
