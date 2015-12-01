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

#include "gpropitem_toolbutton.h"

// ----------------------------------------------------------------------------
// GPropItemObjList
// ----------------------------------------------------------------------------
struct GPropItemObjList : public GPropItemToolButton {
  // Q_OBJECT // gilgil temp 2015.12.02

public:
  GPropItemObjList(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro);
  void update() override {}
};

#endif // QT_GUI_LIB
