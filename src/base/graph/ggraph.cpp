#ifdef QT_GUI_LIB

#include "ggraph.h"

// ----------------------------------------------------------------------------
// GGraph
// ----------------------------------------------------------------------------
GGraph::Node* GGraph::Nodes::findNode(QString objectName) {
  foreach (GGraph::Node* node, *this) {
    if (node->objectName() == objectName)
      return node;
  }
  return nullptr;
}

void GGraph::Nodes::clear() {
  foreach (GGraph::Node* node, *this) {
    delete node;
  }
  QList::clear();
}

void GGraph::Nodes::load(QJsonArray ja) {
  clear();
  foreach (QJsonValue jv, ja) {
    QJsonObject nodeJo = jv.toObject();
    QString className = nodeJo["_class"].toString();
    if (className == "") {
      qWarning() << QString("className is empty");
      continue;
    }
    GGraph::Node* node = GGraph::createInstance(className);
    if (node == nullptr) {
      qWarning() << QString("node is null for (%1)").arg(className);
      continue;
    }
    node->propLoad(nodeJo);
    append(node);
  }
}

void GGraph::Nodes::save(QJsonArray& ja) {
  ja = QJsonArray(); // clear
  foreach (GGraph::Node* node, *this) {
    QJsonObject nodeJo;
    node->propSave(nodeJo);
    QString className = node->metaObject()->className();
    nodeJo["_class"] = className;
    ja.append(nodeJo);
  }
}

// ----- gilgil temp 2016.09.23 -----
/*
bool GGraph::Connection::operator ==(const Connection& other) {
  if (sender_ != other.sender_) return false;
  if (signal_ != other.signal_) return false;
  if (receiver_ != other.receiver_) return false;
  if (slot_ != other.slot_) return false;
  return true;
}
*/
// ----------------------------------

void GGraph::Connections::clear() {
  foreach (Connection* connection, *this) {
    delete connection;
  }
  QList::clear();
}

void GGraph::Connections::load(GGraph* graph, QJsonArray ja) {
  clear();
  foreach (QJsonValue jv, ja) {
    QJsonObject connectionJo = jv.toObject();

    QString senderObjectName = connectionJo["sender"].toString();
    GGraph::Node* sender = graph->nodes_.findNode(senderObjectName);
    if (sender == nullptr) {
      qWarning() << QString("can not find node for %1").arg(senderObjectName);
      continue;
    }
    QString signal = connectionJo["signal"].toString();
    QString receiverObjectName = connectionJo["receiver"].toString();
    GGraph::Node* receiver = graph->nodes_.findNode(receiverObjectName);
    if (receiver == nullptr) {
      qWarning() << QString("can not find node for %1").arg(receiverObjectName);
      continue;
    }
    QString slot = connectionJo["slot"].toString();

    bool res = GObj::connect(sender, qPrintable(signal), receiver, qPrintable(slot));
    if (!res) continue;

    Connection* connection = new Connection;
    connection->sender_ = sender;
    connection->signal_ = signal;
    connection->receiver_ = receiver;
    connection->slot_ = slot;
    push_back(connection);
  }
}

void GGraph::Connections::save(QJsonArray& ja) {
  ja = QJsonArray(); // clear
  foreach (Connection* connection, *this) {
    QJsonObject connectionJo;
    connectionJo["sender"] = connection->sender_->objectName();
    connectionJo["signal"] = connection->signal_;
    connectionJo["receiver"] = connection->receiver_->objectName();
    connectionJo["slot"] = connection->slot_;
    ja.append(connectionJo);
  }
}

bool GGraph::doOpen() {
  foreach (Node* node, nodes_) {
    GStateObj* stateObj = dynamic_cast<GStateObj*>(node);
    if (stateObj != nullptr) {
      bool res = stateObj->open();
      if (!res) {
        QString msg = QString("%1(%2)").arg(stateObj->err->msg(), stateObj->objectName());
        SET_ERR(stateObj->err->code(), msg);
        doClose();
        return false;
      }
    }
  }
  return true;
}

bool GGraph::doClose() {
  foreach(Node* node, nodes_) {
    GStateObj* stateObj = dynamic_cast<GStateObj*>(node);
    if (stateObj != nullptr) {
      stateObj->close();
    }
  }
  return true;
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

void GGraph::clear() {
  nodes_.clear();
  connections_.clear();
}

void GGraph::start() {
  doOpen();
}

void GGraph::stop() {
  doClose();
}

void GGraph::propLoad(QJsonObject jo) {
  nodes_.load(jo["nodes"].toArray());
  connections_.load(this, jo["connections"].toArray());
}

void GGraph::propSave(QJsonObject& jo) {
  QJsonArray ja;
  nodes_.save(ja);
  jo["nodes"] = ja;
  connections_.save(ja);
  jo["connections"] = ja;
}

#endif // QT_GUI_LIB
