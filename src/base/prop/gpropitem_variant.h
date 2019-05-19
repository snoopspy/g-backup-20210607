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
// GPropItemVariant
// ----------------------------------------------------------------------------
struct GPropItemVariant : public GPropItemLineEdit {
  Q_OBJECT

public:
  GPropItemVariant(GPropItemParam* param);
  void update() override;

protected slots:
  void myEditingFinished();
};

#endif // QT_GUI_LIB
