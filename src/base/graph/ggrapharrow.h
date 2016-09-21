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
// GGraphArrow
// ----------------------------------------------------------------------------
struct Node;
struct GGraphArrow : QGraphicsLineItem
{
public:
	enum { Type = UserType + 4 };
  GGraphArrow(Node *startNode, QString signal, Node *endNode, QString slot);
  virtual ~GGraphArrow();

  int type() const override { return Type; }
  QRectF boundingRect() const override;
  QPainterPath shape() const override;
	void setColor(const QColor &color) { myColor = color;    }
	Node *startItem() const            { return myStartNode; }
	Node *endItem() const              { return myEndNode;   }

	void updatePosition();

protected:
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:
	Node*     myStartNode;
	Node*     myEndNode;
	QColor    myColor;
	QPolygonF arrowHead;

  GGraph::Connection connection_;
};
