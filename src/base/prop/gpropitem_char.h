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
// GPropItemLineEditQChar
// ----------------------------------------------------------------------------
struct GPropItemLineEditQChar : public GPropItemLineEdit {
  Q_OBJECT

public:
  GPropItemLineEditQChar(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro);
  void update() override;

protected slots:
  void myEditingFinished();
};

#endif // QT_GUI_LIB
