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

void GGraph::Factory::propLoad(QJsonObject jo) {
  removePrefixNames_ = jo["removePrefixNames"].toString().split(",");
  ignoreSignalNames_ = jo["ignoreSignalNames"].toString().split(",");
  ignoreSlotNames_ = jo["ignoreSlotNames"].toString().split(",");
}

void GGraph::Factory::propSave(QJsonObject& jo) {
  jo["removePrefixNames"] = removePrefixNames_.join(",");
  jo["ignoreSignalNames"] = ignoreSignalNames_.join(",");
  jo["ignoreSlotNames"] = ignoreSlotNames_.join(",");
}

void GGraph::clear() {
  nodes_.clear();
  connections_.clear();
}

void GGraph::propLoad(QJsonObject jo) {
  Factory* _factory = factory();
  if (_factory != nullptr)
    jo["factory"] >> *_factory;
  jo["nodes"] >> nodes_;
  jo["connections"] >> connections_;
}

void GGraph::propSave(QJsonObject& jo) {
  Factory* _factory = factory();
  if (_factory != nullptr)
    jo["factory"] << *_factory;
  jo["nodes"] << nodes_;
  jo["connections"] << connections_;
}
