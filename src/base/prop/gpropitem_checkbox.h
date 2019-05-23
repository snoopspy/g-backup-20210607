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

#include <QCheckBox>
#include "gpropitem.h"

// ----------------------------------------------------------------------------
// GPropItemCheckBox
// ----------------------------------------------------------------------------
struct G_EXPORT GPropItemCheckBox : public GPropItem {
  GPropItemCheckBox(GPropItemParam* param);

public:
  QCheckBox* checkBox_{nullptr};
};

#endif // QT_GUI_LIB
