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
// GPropItemVariantSelect
// ----------------------------------------------------------------------------
struct GPropItemVariantSelect : public GPropItemComboBox {
  Q_OBJECT

public:
  GPropItemVariantSelect(GPropItemParam* param);
  GPropItemVariantSelect(GPropItemParam* param, QStringList& items);
  void update() override;

protected slots:
  void myCurrentIndexChanged(const QString &arg1);
};

#endif // QT_GUI_LIB
