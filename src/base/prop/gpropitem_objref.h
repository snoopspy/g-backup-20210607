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
// GPropItemObjRef
// ----------------------------------------------------------------------------
struct GPropItemObjRef : public GPropItem {
  Q_OBJECT

public:
  GPropItemObjRef(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro);
  void update() override {}
};

#endif // QT_GUI_LIB
