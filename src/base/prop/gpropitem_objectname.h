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

#include "gpropitem_lineedit.h"

// ----------------------------------------------------------------------------
// GPropItemObjectName
// ----------------------------------------------------------------------------
struct GPropItemObjectName : public GPropItemLineEdit {
  Q_OBJECT

public:
  GPropItemObjectName(QTreeWidget* widget, QObject* object, QMetaProperty mpro);
  GPropItemObjectName(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro);
  void update() override;

protected slots:
  void myEditingFinished();
};

#endif // QT_GUI_LIB
