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

#include <QLineEdit>
#include "gpropitem.h"

// ----------------------------------------------------------------------------
// GPropItemLineEdit
// ----------------------------------------------------------------------------
struct GPropItemLineEdit : public GPropItem {
  void init(GPropItemParam param) override;

public:
  QLineEdit* lineEdit_{nullptr};
};

#endif // QT_GUI_LIB
