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

GGraph::Node* GGraph::Factory::createInstance(QString className) {
  QObject* obj = GObj::createInstance(className);
  if (obj == nullptr) return nullptr;
  Node* node = dynamic_cast<Node*>(obj);
  if (node == nullptr) {
    qWarning() << QString("dynamic_cast<Node*> failed for (%1)").arg(className);
  }

  QString objectName = node->metaObject()->className();
  foreach (QString removePrefixName, removePrefixNames_) {
    if (objectName.startsWith(removePrefixName))
      objectName = objectName.mid(removePrefixName.length());
  }
  if (objectName == "") {
    qWarning() << QString("objectName is empty for (%1)").arg(className);
    delete node;
    return nullptr;
  }
  if (toLowerFirstCharacter_) {
    char first = objectName.at(0).toLatin1();
    if (first >= 'A' && first <= 'Z') {
      first = first + 'a' - 'A';
      objectName = QString(first) + objectName.mid(1);
    }
  }

  int suffix = 1;
  while (true) {
    QString _objectName = objectName + QString::number(suffix);
    GGraph::Node* exist = graph_->findChild<GGraph::Node*>(_objectName);
    if (exist == nullptr)
      break;
    suffix++;
  }
  objectName = objectName + QString::number(suffix);
  node->setObjectName(objectName);

  return node;
}

void GGraph::Factory::propLoad(QJsonObject jo) {
  toLowerFirstCharacter_ = jo["toLowerFirstCharacter"].toBool();
  removePrefixNames_ = jo["removePrefixNames"].toString().split(",");
  ignoreSignalNames_ = jo["ignoreSignalNames"].toString().split(",");
  ignoreSlotNames_ = jo["ignoreSlotNames"].toString().split(",");
}

void GGraph::Factory::propSave(QJsonObject& jo) {
  jo["toLowerFirstCharacter"] = toLowerFirstCharacter_;
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
