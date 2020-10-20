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

#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QRectF>
#include <QPainter>

#include "gg-arrow.h"

// ----------------------------------------------------------------------------
// GGText
// ----------------------------------------------------------------------------
struct G_EXPORT GGText : QGraphicsTextItem {
public:
	enum { Type = UserType + 5 };
	int type() const override { return Type; }

public:
	GGText(GGraph::Node* node);
	~GGText() override;

public:
	QList<GGArrow*> arrows_;
	void addArrow(GGArrow *arrow);
	void removeArrow(GGArrow *arrow);
	void removeArrows();

public:
	GGraph::Node* node_;

public:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // QT_GUI_LIB
