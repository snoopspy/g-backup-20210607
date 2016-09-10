#include "gparser.h"
#include "net/packet/gpacket.h"
#include "net/pdu/gpdu.h"

// ----------------------------------------------------------------------------
// GParser
// ----------------------------------------------------------------------------
size_t GParser::parse(GPacket* packet) {
  GPdu* pdu = doParse(packet);
  if (pdu == nullptr) return 0;
  size_t res = pdu->size();

  for (GParsers::iterator it = children_.begin(); it != children_.end(); it++) {
    GParser* childParser = *it;
    if (childParser->isMatch(pdu, packet)) {
      res += childParser->parse(packet);
      return res;
    }
  }
  return 0;
}

GParser* GParser::findFirstChild(QString className) {
 for (GParsers::iterator it = children_.begin(); it != children_.end(); it++) {
   GParser* child = *it;
   if (className == child->metaObject()->className())
    return child;
 }
 return nullptr;
}

static void _findAll(GParser* _this, QString className, QVector<GParser*>& res) {
  if (className == QString(_this->metaObject()->className()))
    res.push_back(_this);
  for (GParsers::iterator it = _this->children_.begin(); it != _this->children_.end(); it++) {
    GParser* child = *it;
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
  for (QVector<GParser*>::iterator it = parsers.begin(); it != parsers.end(); it++) {
    GParser* parser = *it;
    childClassName += "*";
    int id = QMetaType::type(qPrintable(childClassName)); // gilgil temp 2016.09.10
    if (id == QMetaType::UnknownType) {
      qCritical() << QString("can not find class type for (%1)").arg(childClassName);
      return;
    }
    const QMetaObject* mobj = QMetaType::metaObjectForType(id);
    QObject* obj = mobj->newInstance();
    if (obj == nullptr) {
      qCritical() << QString("can not create class for (%1)").arg(childClassName);
      return;
    }
    GParser* child = dynamic_cast<GParser*>(obj);
    if (child == nullptr) {
      qCritical() << QString("maybe not gparser class for (%1)").arg(childClassName);
      return;
    }
    parser->children_.push_back(child);
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
