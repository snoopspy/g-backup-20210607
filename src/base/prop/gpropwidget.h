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
struct G_EXPORT GPropWidget : public QTreeWidget, GProp {
  GPropWidget(QWidget *parent = nullptr);
  GPropWidget(QObject* object);
  ~GPropWidget() override;

public:
  void init();
  QObject* object() { return object_; }
  void setObject(QObject* object);
  void update();
  void clear();

protected:
  bool isFirstSetObject_{true};
  QObject* object_{nullptr};

public:
  void propLoad(QJsonObject jo) override;
  void propSave(QJsonObject& jo) override;
};

#endif // QT_GUI_LIB
