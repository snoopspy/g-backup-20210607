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
// GPropItemChar
// ----------------------------------------------------------------------------
struct GPropItemChar : public GPropItemLineEdit {
  Q_OBJECT

public:
  GPropItemChar(GPropItemParam param);
  void update() override;

protected slots:
  void myEditingFinished();
};

#endif // QT_GUI_LIB
