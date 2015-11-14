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

#include "gpropitemcombobox.h"

// ----------------------------------------------------------------------------
// GPropItemComboBoxQVariant
// ----------------------------------------------------------------------------
struct GPropItemComboBoxQVariant : public GPropItemComboBox {
  Q_OBJECT

public:
  GPropItemComboBoxQVariant(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro, QStringList items);
  void update() override;

protected slots:
  void myCurrentIndexChanged(const QString &arg1);
};

#endif // QT_GUI_LIB
