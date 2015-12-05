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

#include <QComboBox>
#include "gpropitem.h"

// ----------------------------------------------------------------------------
// GPropItemWidget
// ----------------------------------------------------------------------------
struct GPropItemWidget : public GPropItem {
  void init(GPropItemParam param) override;

public:
  QWidget* widget_{nullptr};
};

#endif // QT_GUI_LIB
