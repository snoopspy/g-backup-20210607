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
// GPropItemObjVector
// ----------------------------------------------------------------------------
struct GPropItemObjVector : public GPropItemToolButton {
  GPropItemObjVector(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro);
  void update() override {}
};


#endif // QT_GUI_LIB
