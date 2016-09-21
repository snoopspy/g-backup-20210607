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
#include "gg_scene.h"

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
  void clear();

protected:
  void updateFactory(GGraph::Factory::Item* item, QTreeWidgetItem* parent);

public:
  GGraph::Node* createInstance(QString className);
  GGraph::Node* createNodeIfItemNodeSelected();

protected:
  GGraph* graph_{nullptr};

public:
  bool toLowerFirstCharacter_{true};
  QStringList removePrefixNames_{"G"};
  QStringList ignoreSignalNames_{"destroyed(QObject*)", "destroyed()", "objectNameChanged(QString)"};
  QStringList ignoreSlotNames_{"deleteLater()"};

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
  GGScene*         scene_;
  QGraphicsView* graphView_;
  QStatusBar*  statusBar_;

public slots:
  void setControl();

public slots:
  void actionStartTriggered(bool);
  void actionStopTriggered(bool);
  void actionEditTriggered(bool);
  void actionLinkTriggered(bool);
  void actionDeleteTriggered(bool);
  void actionOptionTriggered(bool);

// ----- gilgil temp 2016.09.20 -----
public slots:
  void factoryWidgetClicked(const QModelIndex&);
// ----------------------------------
};

#endif // QT_GUI_LIB
