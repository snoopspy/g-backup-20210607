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
// GPropItemObjPtrListPtr
// ----------------------------------------------------------------------------
struct GPropItemObjPtrListPtr : public GPropItemToolButton {
  Q_OBJECT

public:
  void init(GPropItemParam param) override;
  void update() override;

protected slots:
  void myClicked(bool checked);
};

#endif // QT_GUI_LIB
