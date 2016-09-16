#ifdef QT_GUI_LIB

#include "ggraphwidget.h"

#include "base/gjson.h"

// ----------------------------------------------------------------------------
// GGraphWidget
// ----------------------------------------------------------------------------
GGraphWidget::GGraphWidget(QWidget *parent) : QWidget(parent) {
  init();
}

GGraphWidget::GGraphWidget(GGraph* graph) {
  init();
  setGraph(graph);
}

GGraphWidget::~GGraphWidget() {
}

void GGraphWidget::init() {

}

void GGraphWidget::setGraph(GGraph* graph) {
  if (graph == graph_) return;
  // clear(); // gilgil temp 2016.09.17

  // gilgil temp 2016.09.17

  graph_ = graph;
}

void GGraphWidget::update() {

}

void GGraphWidget::propLoad(QJsonObject jo) {
  jo["rect"] >> GJson::rect(this);
  // jo["sizes"] >> GJson::headerSizes(this); // gilgil temp 2016.09.17
}

void GGraphWidget::propSave(QJsonObject& jo) {
  jo["rect"] << GJson::rect(this);
  // jo["sizes"] << GJson::headerSizes(this); // gilgil temp 2016.09.17
}

#endif // QT_GUI_LIB
