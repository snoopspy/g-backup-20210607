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
// GPropItemComboBox
// ----------------------------------------------------------------------------
struct GPropItemComboBox : public GPropItem {
  GPropItemComboBox(GPropItemParam param);

public:
  QComboBox* comboBox_{nullptr};
};

#endif // QT_GUI_LIB
