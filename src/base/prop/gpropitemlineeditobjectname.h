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

#include "gpropitemlineedit.h"

// ----------------------------------------------------------------------------
// GPropItemLineEditObjectName
// ----------------------------------------------------------------------------
struct GPropItemLineEditObjectName : public GPropItemLineEdit {
  Q_OBJECT

public:
  GPropItemLineEditObjectName(QTreeWidget* widget, QObject* object, QMetaProperty mpro);
  GPropItemLineEditObjectName(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro);
  void update() override;

protected slots:
  void myEditingFinished();
};

#endif // QT_GUI_LIB
