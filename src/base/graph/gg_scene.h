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

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QTreeWidget>
#include <QMessageBox>

#include "gg_arrow.h"
#include "gg_text.h"
#include "gg_signalslotform.h"

// ----------------------------------------------------------------------------
// Scene
// ----------------------------------------------------------------------------
struct GGraphWidget;
struct GGScene : QGraphicsScene
{
	Q_OBJECT

public:
  GGScene(QObject *parent);
  virtual ~GGScene();

public:
	void clear();

public:
	enum Mode { InsertItem, MoveItem, InsertLine };
	Mode mode() { return m_mode; }
	void setMode(Mode mode) { m_mode = mode; }

protected:
	Mode m_mode;

protected:
  // QTreeWidget* treeWidget; // gilgil temp 2016.09.20

// ----- gilgil temp 2016.09.20 -----
/*
public:
	QStringList categoryNames;
	QStringList removePrefixNames;
	QStringList removeSignalNames;
	QStringList removeSlotNames;
*/
// ----------------------------------

// ----- gilgil temp 2016.09.20 -----
/*
public: // for treeWidget
	void setTreeWidget(QTreeWidget* treeWidget) { this->treeWidget = treeWidget; }
	void addClass(QString categoryName);
	void addClass(VMetaClass* parentMetaClass, QTreeWidgetItem* parentItem);
	void addClasses();
*/
// ----------------------------------

public:
  GGraphWidget* graphWidget_;
  GGSignalSlotForm* signalSlotForm_; // gilgil temp 2016.09.20

public:
  // QString generateObjectClassName(QString className);
  // GGText*  createNode(QString className, QString name, bool createObject); // gilgil temp 2016.09.22
  GGArrow* createArrow(GGText* startText, QString signal, GGText* endText, QString slot);
  GGArrow* createArrow(QString startTextName, QString signal, QString endTextName, QString slot);
  GGText*  findTextByObjectName(QString objectName);

public: // for file manipulation
  //bool newFile(QString& errStr); // gilgil temp 2012.07.27
  //bool loadFromFile(QString fileName, QString& errStr); // gilgil temp 2012.07.27
  //bool saveToFile(QString fileName, QString& errStr); // gilgil temp 2012.07.27

protected: // for drag and drop
	QGraphicsLineItem *line;

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

// ----- gilgil temp 2016.09.20 -----
/*
public:
	virtual void load(VXml xml);
	virtual void save(VXml xml);
*/
// ----------------------------------
};
