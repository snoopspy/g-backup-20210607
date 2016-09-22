#include "gg_arrow.h"
#include "gg_text.h"
#include "gg_scene.h"
#include "ggraphwidget.h"

const qreal Pi = 3.14;

GGArrow::GGArrow(GGText *startText, QString signal, GGText *endText, QString slot) : QGraphicsLineItem(nullptr)
{
  myStartText = startText;
  myEndText   = endText;

	setFlag(QGraphicsItem::ItemIsSelectable, true);
	myColor = Qt::black;
	setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	setZValue(-1000.0);

  connection_.sender_   = startText->obj_->objectName();
  connection_.signal_   = signal;
  connection_.receiver_ = endText->obj_->objectName();
  connection_.slot_     = slot;
}

GGArrow::~GGArrow()
{
  GGScene* scene = (GGScene*)this->scene();
  GGraph::Connections& connections = scene->graphWidget_->graph()->connections_;
  int index = connections.indexOf(connection_);
  Q_ASSERT(index != -1);
  connections.removeAt(index);
}

QRectF GGArrow::boundingRect() const
{
  qreal extra = (pen().width() + 20) / 2.0;

  return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
    line().p2().y() - line().p1().y())).normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath GGArrow::shape() const
{
	QPainterPath path = QGraphicsLineItem::shape();
  path.addPolygon(arrowHead);
	return path;
}

void GGArrow::updatePosition()
{
  QLineF line(mapFromItem(myStartText, 0, 0), mapFromItem(myEndText, 0, 0));
	setLine(line);
}

void GGArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
					QWidget *)
{
  if (myStartText->collidesWithItem(myEndText))
			return;

  QPen myPen = pen();
  myPen.setColor(myColor);
  qreal ArrowSize = 10;
  painter->setPen(myPen);
  painter->setBrush(myColor);

  // ----- gilgil temp 2012.06.29 -----
  /*
  QLineF centerLine(myStartNode->pos(), myEndNode->pos());
  QPolygonF endPolygon = myEndNode->polygon();
  QPointF p1 = endPolygon.first() + myEndNode->pos();
  QPointF p2;
  QPointF intersectPoint;
  QLineF polyLine;
  for (int i = 1; i < endPolygon.count(); ++i) {
  p2 = endPolygon.at(i) + myEndNode->pos();
  polyLine = QLineF(p1, p2);
  QLineF::IntersectType intersectType =
      polyLine.intersect(centerLine, &intersectPoint);
  if (intersectType == QLineF::BoundedIntersection)
      break;
      p1 = p2;
  }
  */
  // ----------------------------------
  QPointF start, end;
  start.setX(myStartText->pos().x() + myStartText->boundingRect().width() / 2);
  start.setY(myStartText->pos().y() + myStartText->boundingRect().height() / 2);
  end.setX  (myEndText->pos().x()   + myEndText->boundingRect().width() / 2);
  end.setY  (myEndText->pos().y()   + myEndText->boundingRect().height() / 2);
  QLineF centerLine(start, end);

  QPointF intersectPoint;
  QLineF::IntersectType intersectType;
  QLineF polyLine;

  while (true)
  {
    polyLine = QLineF(myEndText->sceneBoundingRect().topLeft(), myEndText->sceneBoundingRect().topRight());
    intersectType = polyLine.intersect(centerLine, &intersectPoint);
    if (intersectType == QLineF::BoundedIntersection) break;

    polyLine = QLineF(myEndText->sceneBoundingRect().topLeft(), myEndText->sceneBoundingRect().bottomLeft());
    intersectType = polyLine.intersect(centerLine, &intersectPoint);
    if (intersectType == QLineF::BoundedIntersection) break;

    polyLine = QLineF(myEndText->sceneBoundingRect().topRight(), myEndText->sceneBoundingRect().bottomRight());
    intersectType = polyLine.intersect(centerLine, &intersectPoint);
    if (intersectType == QLineF::BoundedIntersection) break;

    polyLine = QLineF(myEndText->sceneBoundingRect().bottomLeft(), myEndText->sceneBoundingRect().bottomRight());
    intersectType = polyLine.intersect(centerLine, &intersectPoint);
    if (intersectType == QLineF::BoundedIntersection) break;

    break;
  }

  setLine(QLineF(intersectPoint, start));

  double angle = ::acos(line().dx() / line().length());
  if (line().dy() >= 0)
    angle = (Pi * 2) - angle;

    QPointF ArrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * ArrowSize, cos(angle + Pi / 3) * ArrowSize);
    QPointF ArrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * ArrowSize, cos(angle + Pi - Pi / 3) * ArrowSize);

    arrowHead.clear();
    arrowHead << line().p1() << ArrowP1 << ArrowP2;

    painter->drawLine(line());
    painter->drawPolygon(arrowHead);
    if (isSelected()) {
      painter->setPen(QPen(myColor, 1, Qt::DashLine));
    QLineF myLine = line();
    myLine.translate(0, 4.0);
    painter->drawLine(myLine);
    myLine.translate(0,-8.0);
    painter->drawLine(myLine);
  }
}
