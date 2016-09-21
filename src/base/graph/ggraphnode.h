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

#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QRectF>
#include <QPainter>

#include "base/gobj.h"
#include "ggrapharrow.h"

// ----------------------------------------------------------------------------
// Node
// ----------------------------------------------------------------------------
class Node : public QGraphicsTextItem
{
public:
	enum { Type = UserType + 5 };
	int type() const { return Type; }

public:
  Node(GObj* obj);
	virtual ~Node();

public:
  QList<GGraphArrow*> arrows;
  void addArrow(GGraphArrow *arrow);
  void removeArrow(GGraphArrow *arrow);
	void removeArrows();

public:
  GObj* obj_; // reference

public:
  // virtual QRectF boundingRect() const; // gilgil temp 2016.09.20
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};
