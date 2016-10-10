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
// GPropItemObjPtr
// ----------------------------------------------------------------------------
struct GPropItemObjPtr : public GPropItemComboBox {
  Q_OBJECT

public:
  GPropItemObjPtr(GPropItemParam param);
  void update() override;

protected slots:
  void myCurrentIndexChanged(int index);
};

#endif // QT_GUI_LIB
