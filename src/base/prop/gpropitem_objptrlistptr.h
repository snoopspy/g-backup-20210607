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

#include "gpropitem_toolbutton.h"

// ----------------------------------------------------------------------------
// GPropItemObjPtrList
// ----------------------------------------------------------------------------
struct GPropItemObjPtrList : public GPropItemToolButton {
  Q_OBJECT

public:
  GPropItemObjPtrList(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro);
  void update() override;

protected slots:
  void myClicked(bool checked);
};

#endif // QT_GUI_LIB
