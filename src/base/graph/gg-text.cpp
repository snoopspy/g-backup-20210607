#ifdef QT_GUI_LIB

#include "gg-text.h"
#include "gg-scene.h"
#include "ggraphwidget.h"

GGText::GGText(GObj* node) {
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	node_ = node;
}

GGText::~GGText() {
}

void GGText::addArrow(GGArrow* arrow) {
	arrows_.append(arrow);
}

void GGText::removeArrow(GGArrow* arrow) {
	int index = arrows_.indexOf(arrow);
	if (index != -1)
		arrows_.removeAt(index);
}

void GGText::removeArrows() {
	for (GGArrow* arrow: arrows_) {
		arrow->startText()->removeArrow(arrow);
		arrow->endText()->removeArrow(arrow);
		delete arrow;
	}
}

void GGText::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	painter->drawRect(boundingRect());
	painter->fillRect(boundingRect(), Qt::gray);
	QGraphicsTextItem::paint(painter, option, widget);
}

#endif // QT_GUI_LIB
