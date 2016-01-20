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

#include <QTreeWidget>
#include "base/gobj.h"

// ----------------------------------------------------------------------------
// GPropWidget
// ----------------------------------------------------------------------------
struct GPropWidget : public QTreeWidget {
  GPropWidget(QWidget *parent = 0);
  GPropWidget(QObject* object);
  ~GPropWidget() override;

protected:
  void init();

public:
  QObject* object();
  void setObject(QObject* object);
  void update();

protected:
  bool isFirstSetObject_{true};
  QObject* object_{nullptr};
};

#endif // QT_GUI_LIB
