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
// GPropItemObjPtrVectorPtr
// ----------------------------------------------------------------------------
struct GPropItemObjPtrVectorPtr : public GPropItemToolButton {
  GPropItemObjPtrVectorPtr(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro);
  void update() override {}
};


#endif // QT_GUI_LIB
