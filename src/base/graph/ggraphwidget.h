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

#include <QAction>
#include <QHBoxLayout>
#include <QToolBar>
#include <QSplitter>
#include <QStatusBar>
#include <QTreeWidget>
#include <QTreeView>
#include <QVBoxLayout>

#include "base/prop/gpropwidget.h"

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

protected:
  QAction* actionStart_;
  QAction* actionStop_;
  QAction* actionEdit_;
  QAction* actionLink_;
  QAction* actionDelete_;
  QAction* actionOption_;

  QVBoxLayout* mainLayout_;
  QToolBar*    toolBar_;
  QSplitter*   midSplitter_;
  QSplitter*     midLeftSplitter_;
  QTreeWidget*     nodeFactoryWidget_;
  GPropWidget*     propWidget_;
  QWidget*       midRightWidget_;
  QStatusBar*  statusBar_;

public slots:
  void actionStartTriggered(bool checked);
  void actionStopTriggered(bool checked);
  void actionEditTriggered(bool checked);
  void actionLinkTriggered(bool checked);
  void actionDeleteTriggered(bool checked);
  void actionOptionTriggered(bool checked);
};

#endif // QT_GUI_LIB
