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

#include <QToolButton>
#include "gpropitem.h"

// ----------------------------------------------------------------------------
// GPropItemToolButton
// ----------------------------------------------------------------------------
struct GPropItemToolButton : public GPropItem {
  GPropItemToolButton(GPropItemParam* param);

public:
  QToolButton* toolButton_{nullptr};
};

#endif // QT_GUI_LIB
