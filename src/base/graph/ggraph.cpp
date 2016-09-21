#include "ggraph.h"

// ----------------------------------------------------------------------------
// GGraph
// ----------------------------------------------------------------------------
void GGraph::Nodes::load(QJsonArray ja) {
  clear();
  foreach (QJsonValue jv, ja) {
    QJsonObject childJo = jv.toObject();
    QString className = childJo["class"].toString();
    if (className == "") {
      qWarning() << QString("className is empty");
      continue;
    }
    GGraph::Node* node = GGraph::createInstance(className);
    if (node == nullptr) {
      qWarning() << QString("node is null for (%1)").arg(className);
      continue;
    }
    node->propLoad(childJo);
    append(node);
  }
}

void GGraph::Nodes::save(QJsonArray& ja) {
  ja = QJsonArray(); // clear
  foreach (GGraph::Node* node, *this) {
    QJsonObject childJo;
    node->propSave(childJo);
    QString className = node->metaObject()->className();
    childJo["class"] = className;
    ja.append(childJo);
  }
}

bool GGraph::Connection::operator ==(const Connection& other) {
  if (sender_ != other.sender_) return false;
  if (signal_ != other.signal_) return false;
  if (receiver_ != other.receiver_) return false;
  if (slot_ != other.slot_) return false;
  return true;
}

void GGraph::Connections::load(QJsonArray ja) {
  clear();
  foreach (QJsonValue jv, ja) {
    QJsonObject connectionJo = jv.toObject();
    Connection connection;
    connection.sender_ = connectionJo["sender_"].toString();
    connection.signal_ = connectionJo["signal_"].toString();
    connection.receiver_ = connectionJo["receiver_"].toString();
    connection.slot_ = connectionJo["slot_"].toString();
  }
}

void GGraph::Connections::save(QJsonArray& ja) {
  ja = QJsonArray(); // clear
  foreach (Connection connection, *this) {
    QJsonObject connectionJo;
    connectionJo["sender"] = connection.sender_;
    connectionJo["signal"] = connection.signal_;
    connectionJo["receiver"] = connection.receiver_;
    connectionJo["slot"] = connection.slot_;
    ja.append(connectionJo);
  }
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
  nodes_.load(jo["nodes"].toArray());
  connections_.load(jo["connections"].toArray());
}

void GGraph::propSave(QJsonObject& jo) {
  QJsonArray ja;
  nodes_.save(ja);
  jo["nodes"] = ja;
  connections_.save(ja);
  jo["connections"] = ja;
}
