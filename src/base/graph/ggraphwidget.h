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

#include <QAction>
#include <QHBoxLayout>
#include <QToolBar>
#include <QSplitter>
#include <QStatusBar>
#include <QTreeWidget>
#include <QTreeView>
#include <QVBoxLayout>
#include <QGraphicsView>

#include "base/prop/gpropwidget.h"
#include "ggraphscene.h"

// ----------------------------------------------------------------------------
// GGraphWidget
// ----------------------------------------------------------------------------
struct GGraphWidget : public QWidget, GProp {
public:
  GGraphWidget(QWidget *parent = nullptr);
  ~GGraphWidget() override;

public:
  void init();
  GGraph* graph() { return graph_; }
  void setGraph(GGraph* graph);
  void update();

protected:
  void updateFactory(GGraph::Factory::Item* item, QTreeWidgetItem* parent);

public:
  GGraph::Node* createNodeIfItemNodeSelected();

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
  QTreeWidget*     factoryWidget_;
  GPropWidget*     propWidget_;
  Scene*         scene_;
  QGraphicsView* graphView_;
  QStatusBar*  statusBar_;

public slots:
  void actionStartTriggered(bool checked);
  void actionStopTriggered(bool checked);
  void actionEditTriggered(bool checked);
  void actionLinkTriggered(bool checked);
  void actionDeleteTriggered(bool checked);
  void actionOptionTriggered(bool checked);

// ----- gilgil temp 2016.09.20 -----
public slots:
  void factoryWidgetClicked(const QModelIndex &index);
  void doubleClicked(const QModelIndex &index);
// ----------------------------------
};

#endif // QT_GUI_LIB
