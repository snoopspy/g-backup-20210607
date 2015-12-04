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

#include "gpropitem.h"

// ----------------------------------------------------------------------------
// GPropItemObjPtr
// ----------------------------------------------------------------------------
struct GPropItemObjPtr : public GPropItem {
  void init(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, QObject* object, QMetaProperty mpro) override;
  void update() override {}
};

#endif // QT_GUI_LIB
