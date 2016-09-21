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


#include <QGraphicsLineItem>
#include <QPen>
//#include <QPolygonF>
#include "ggraph.h"

// ----- gilgil temp 2016.09.20 -----
/*
QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE
*/
// ----------------------------------

// ----------------------------------------------------------------------------
// GGArrow
// ----------------------------------------------------------------------------
struct GGNode;
struct GGArrow : QGraphicsLineItem
{
public:
	enum { Type = UserType + 4 };
  GGArrow(GGNode *startNode, QString signal, GGNode *endNode, QString slot);
  virtual ~GGArrow();

  int type() const override { return Type; }
  QRectF boundingRect() const override;
  QPainterPath shape() const override;
	void setColor(const QColor &color) { myColor = color;    }
  GGNode *startItem() const            { return myStartNode; }
  GGNode *endItem() const              { return myEndNode;   }

	void updatePosition();

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
  GGNode*     myStartNode;
  GGNode*     myEndNode;
	QColor    myColor;
	QPolygonF arrowHead;

  GGraph::Connection connection_;
};
