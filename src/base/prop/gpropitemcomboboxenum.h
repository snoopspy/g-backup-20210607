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

#include <QMetaEnum>
#include "gpropitemcombobox.h"

// ----------------------------------------------------------------------------
// GPropItemComboBoxEnum
// ----------------------------------------------------------------------------
struct GPropItemComboBoxEnum : public GPropItemComboBox {
  Q_OBJECT

public:
  GPropItemComboBoxEnum(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro);
  void update() override;

protected slots:
  void myCurrentIndexChanged(int index);
};

#endif // QT_GUI_LIB
