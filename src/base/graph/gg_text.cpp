#include "gg_text.h"
#include "gg_scene.h"
#include "ggraphwidget.h"

GGText::GGText(GGraph::Node* node) {
	// LOG_DEBUG("%p scene()=%p", this, this->scene()); // gilgil temp 2012.07.27
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
  node_ = node;
	// LOG_DEBUG("scene=%p", scene()); // gilgil temp 2012.07.27
}

GGText::~GGText() {
}

void GGText::addArrow(GGArrow* arrow)
{
  arrows_.append(arrow);
}

void GGText::removeArrow(GGArrow* arrow)
{
  int index = arrows_.indexOf(arrow);

	if (index != -1)
    arrows_.removeAt(index);
}

void GGText::removeArrows()
{
  foreach (GGArrow* arrow, arrows_)
	{
    arrow->startText()->removeArrow(arrow);
    arrow->endText()->removeArrow(arrow);
		// scene()->removeItem(arrow); // gilgil temp 2012.07.27
		delete arrow;
	}
}

// ----- gilgil temp 2016.09.20 -----
/*
QRectF GGText::boundingRect() const
{
	LOG_DEBUG("boundingRect");
	;
	return QRectF(0, 0, this->textWidth(), 50);
}
*/
// ----------------------------------

void GGText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->drawRect(boundingRect());
	painter->fillRect(boundingRect(), Qt::gray);
	QGraphicsTextItem::paint(painter, option, widget);
}
