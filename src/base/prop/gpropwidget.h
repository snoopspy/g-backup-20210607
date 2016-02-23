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
struct GPropWidget : public QTreeWidget, GProp {
  GPropWidget(QWidget *parent = 0);
  GPropWidget(QObject* object);
  ~GPropWidget() override;

public:
  QObject* object();
  void init();
  void setObject(QObject* object);
  void update();

protected:
  bool isFirstSetObject_{true};
  QObject* object_{nullptr};

  void propLoad(QJsonObject jo) override;
  void propSave(QJsonObject& jo) override;
};

#endif // QT_GUI_LIB
