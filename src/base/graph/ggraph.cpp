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

bool GGraph::Connection::operator ==(const Connection& other) {
  if (sender_ != other.sender_) return false;
  if (signal_ != other.signal_) return false;
  if (receiver_ != other.receiver_) return false;
  if (slot_ != other.slot_) return false;
  return true;
}

void GGraph::Connections::propLoad(QJsonObject jo) {
  (void)jo; // gilgil temp 2016.09.20
}

void GGraph::Connections::propSave(QJsonObject& jo) {
  (void)jo; // gilgil temp 2016.09.20
}

void GGraph::clear() {
  nodes_.clear();
  connections_.clear();
}

GGraph::Node* GGraph::createInstance(QString className) {
  QObject* obj = GObj::createInstance(className);
  if (obj == nullptr) return nullptr;
  Node* node = dynamic_cast<Node*>(obj);
  if (node == nullptr) {
    qWarning() << QString("dynamic_cast<Node*> failed for (%1)").arg(className);
  }
  return node;
}

void GGraph::propLoad(QJsonObject jo) {
  jo["nodes"] >> nodes_;
  jo["connections"] >> connections_;
}

void GGraph::propSave(QJsonObject& jo) {
  jo["nodes"] << nodes_;
  jo["connections"] << connections_;
}
