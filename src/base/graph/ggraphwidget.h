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

#include "ggraph.h"

// ----------------------------------------------------------------------------
// GGraphWidget
// ----------------------------------------------------------------------------
struct GGraphWidget : public QWidget, GProp {
  GGraphWidget(QWidget *parent = nullptr);
  GGraphWidget(GGraph* graph);
  ~GGraphWidget() override;

public:
  void init();
  GGraph* graph() { return graph_; }
  void setGraph(GGraph* graph);
  void update();

protected:
  GGraph* graph_{nullptr};

public:
  void propLoad(QJsonObject jo) override;
  void propSave(QJsonObject& jo) override;
};

#endif // QT_GUI_LIB
