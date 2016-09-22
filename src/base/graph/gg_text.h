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
#include "gg_arrow.h"

// ----------------------------------------------------------------------------
// GGText
// ----------------------------------------------------------------------------
struct GGText : QGraphicsTextItem
{
public:
	enum { Type = UserType + 5 };
	int type() const { return Type; }

public:
  GGText(GObj* obj);
  ~GGText() override;

public:
  QList<GGArrow*> arrows_;
  void addArrow(GGArrow *arrow);
  void removeArrow(GGArrow *arrow);
	void removeArrows();

public:
  GObj* obj_; // reference

public:
  // virtual QRectF boundingRect() const; // gilgil temp 2016.09.20
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};
