// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#ifdef QT_GUI_LIB

#include "gpropitem_combobox.h"

// ----------------------------------------------------------------------------
// GPropItemVariantSelect
// ----------------------------------------------------------------------------
struct GPropItemVariantSelect : public GPropItemComboBox {
  Q_OBJECT

public:
  void init(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, QObject* object, QMetaProperty mpro) override;
  void update() override;

protected slots:
  void myCurrentIndexChanged(const QString &arg1);
};

#endif // QT_GUI_LIB
