#include "ggraph.h"

// ----------------------------------------------------------------------------
// GGraph
// ----------------------------------------------------------------------------
void GGraph::Nodes::propLoad(QJsonObject jo) {
  (void)jo; // gilgil temp 2016.09.20
}

void GGraph::Nodes::propSave(QJsonObject& jo) {
  jo = QJsonObject(); // clear
  foreach (GGraph::Node* node, *this) {
    QJsonObject childJo;
    node->propSave(childJo);
    QString objectName = node->objectName();
    jo[objectName] = childJo; // gilgil temp 2016.09.20
  }
}

void GGraph::Connections::propLoad(QJsonObject jo) {
  (void)jo; // gilgil temp 2016.09.20
}

void GGraph::Connections::propSave(QJsonObject& jo) {
  (void)jo; // gilgil temp 2016.09.20
}

void GGraph::propLoad(QJsonObject jo) {
  jo["nodes"] >> nodes_;
  jo["connections"] >> connections_;
}

void GGraph::propSave(QJsonObject& jo) {
  jo["nodes"] << nodes_;
  jo["connections"] << connections_;
}
