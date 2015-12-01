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

#include "gpropitemcheckbox.h"

// ----------------------------------------------------------------------------
// GPropItemComboBoxBool
// ----------------------------------------------------------------------------
struct GPropItemComboBoxBool : public GPropItemCheckBox {
  Q_OBJECT

public:
  GPropItemComboBoxBool(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro);
  void update() override;

protected slots:
  void myClicked(bool checked);
};

#endif // QT_GUI_LIB
