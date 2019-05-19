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
  GPropItemObjRef(GPropItemParam* param);
  void update() override {}
};

#endif // QT_GUI_LIB
