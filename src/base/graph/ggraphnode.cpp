#include "ggraphnode.h"
#include "ggraphscene.h"
#include "ggraphwidget.h"

Node::Node(GObj* obj)
{
	// LOG_DEBUG("%p scene()=%p", this, this->scene()); // gilgil temp 2012.07.27
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
  obj_ = obj;
	// LOG_DEBUG("scene=%p", scene()); // gilgil temp 2012.07.27
}

Node::~Node()
{
  if (obj_ != nullptr)
	{
		Scene* scene = (Scene*)this->scene();
    GGraph::Nodes& nodes = scene->graphWidget_->graph()->nodes_;
    int index = nodes.indexOf(obj_);
    Q_ASSERT(index != -1);
    nodes.removeAt(index);
    obj_ = nullptr;
	}
}

void Node::addArrow(GGraphArrow* arrow)
{
	arrows.append(arrow);
}

void Node::removeArrow(GGraphArrow* arrow)
{
	int index = arrows.indexOf(arrow);

	if (index != -1)
		arrows.removeAt(index);
}

void Node::removeArrows()
{
  foreach (GGraphArrow* arrow, arrows)
	{
		arrow->startItem()->removeArrow(arrow);
		arrow->endItem()->removeArrow(arrow);
		// scene()->removeItem(arrow); // gilgil temp 2012.07.27
		delete arrow;
	}
}

// ----- gilgil temp 2016.09.20 -----
/*
QRectF Node::boundingRect() const
{
	LOG_DEBUG("boundingRect");
	;
	return QRectF(0, 0, this->textWidth(), 50);
}
*/
// ----------------------------------

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->drawRect(boundingRect());
	painter->fillRect(boundingRect(), Qt::gray);
	QGraphicsTextItem::paint(painter, option, widget);
}
