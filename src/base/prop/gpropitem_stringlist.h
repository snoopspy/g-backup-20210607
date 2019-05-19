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

#include "gpropitem_plaintextedit.h"

// ----------------------------------------------------------------------------
// GPropItemStringList
// ----------------------------------------------------------------------------
struct GPropItemStringList : public GPropItemPlainTextEdit {
  Q_OBJECT

public:
  GPropItemStringList(GPropItemParam* param);
  void update() override;

protected slots:
  void myEditingFinished();
};

#endif // QT_GUI_LIB
