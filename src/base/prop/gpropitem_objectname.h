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

#include "gpropitem_lineedit.h"

// ----------------------------------------------------------------------------
// GPropItemObjectName
// ----------------------------------------------------------------------------
struct GPropItemObjectName : public GPropItemLineEdit {
  Q_OBJECT

public:
  GPropItemObjectName(GPropItemParam param);
  void update() override;

protected slots:
  void myEditingFinished();
};

#endif // QT_GUI_LIB