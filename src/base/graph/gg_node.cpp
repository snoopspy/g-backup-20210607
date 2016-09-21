#include "gg_node.h"
#include "gg_scene.h"
#include "ggraphwidget.h"

GGNode::GGNode(GObj* obj)
{
	// LOG_DEBUG("%p scene()=%p", this, this->scene()); // gilgil temp 2012.07.27
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
  obj_ = obj;
	// LOG_DEBUG("scene=%p", scene()); // gilgil temp 2012.07.27
}

GGNode::~GGNode()
{
  if (obj_ != nullptr)
	{
    GGScene* scene = (GGScene*)this->scene();
    GGraph::Nodes& nodes = scene->graphWidget_->graph()->nodes_;
    int index = nodes.indexOf(obj_);
    Q_ASSERT(index != -1);
    nodes.removeAt(index);
    delete obj_;
    obj_ = nullptr;
	}
}

void GGNode::addArrow(GGArrow* arrow)
{
  arrows_.append(arrow);
}

void GGNode::removeArrow(GGArrow* arrow)
{
  int index = arrows_.indexOf(arrow);

	if (index != -1)
    arrows_.removeAt(index);
}

void GGNode::removeArrows()
{
  foreach (GGArrow* arrow, arrows_)
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

void GGNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->drawRect(boundingRect());
	painter->fillRect(boundingRect(), Qt::gray);
	QGraphicsTextItem::paint(painter, option, widget);
}
