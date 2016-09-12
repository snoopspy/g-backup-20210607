#include "gparser.h"
#include "net/packet/gpacket.h"
#include "net/pdu/gpdu.h"

// ----------------------------------------------------------------------------
// GParser
// ----------------------------------------------------------------------------
size_t GParser::parse(GPacket* packet) {
  GPdu* pdu = doParse(packet);
  if (pdu == nullptr)
    return 0;

  size_t res = pdu->size();
  packet->buf_ += res;
  packet->len_ -= res;
  packet->pdus_.push_back(pdu);

  foreach (QObject* obj, children()) {
    GParser* child = (GParser*)obj;
    if (child->isMatch(pdu, packet)) {
      res += child->parse(packet);
      return res;
    }
  }
  return 0;
}

GParser* GParser::findFirstChild(QString className) {
  foreach (QObject* obj, children()) {
    GParser* child = (GParser*)obj;
    if (className == child->metaObject()->className())
      return child;
  }
 return nullptr;
}

static void _findAll(GParser* _this, QString className, QVector<GParser*>& res) {
  Q_ASSERT(_this != nullptr);
  const QMetaObject* mobj = _this->metaObject();
  Q_ASSERT(mobj != nullptr);
  QString _className = QString(mobj->className());
  if (className == _className)
    res.push_back(_this);
  foreach (QObject* obj, _this->children()) {
    GParser* child = (GParser*)obj;
    _findAll(child, className, res);
  }
}

QVector<GParser*> GParser::findAll(QString className) {
  QVector<GParser*> res;
  _findAll(this, className, res);
  return res;
}

void GParser::addChild(QString myClassName, QString childClassName) {
  QVector<GParser*> parsers = findAll(myClassName);
  foreach (GParser* parser, parsers) {
    QObject* obj = GObj::createInstance(childClassName);
    if (obj == nullptr) {
      qCritical() << QString("can not createInstance for (%1)").arg(childClassName);
      continue;
    }

    GParser* child = dynamic_cast<GParser*>(obj);
    if (child == nullptr) {
      qCritical() << QString("maybe not gparser class for (%1)").arg(childClassName);
      continue;
    }
    child->setParent(parser);
  }
}

bool GParser::isMatch(GPdu* prev, GPacket* packet) {
  (void)prev;
  (void)packet;
  SET_ERR(GErr::VIRTUAL_FUNCTION_CALL, "virtual function call");
  return false;
}

GPdu* GParser::doParse(GPacket* packet) {
  (void)packet;
  SET_ERR(GErr::VIRTUAL_FUNCTION_CALL, "virtual function call");
  return false;
}
