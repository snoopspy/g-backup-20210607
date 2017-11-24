// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include <QMap>
#include "base/gobj.h"
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GParsers
// ----------------------------------------------------------------------------
struct GParser;
typedef QVector<GParser*> GParsers;

// ----------------------------------------------------------------------------
// GParser
// ----------------------------------------------------------------------------
struct GParser : GObj {
  Q_OBJECT

public:
  GParser(QObject* parent = nullptr) : GObj(parent) {}
  ~GParser() override {}

public:
  // ----- gilgil temp 2017.11.24 -----
  /*
  GParser* findFirstChild(QString className);
  QVector<GParser*> findAll(QString className);
  void addChild(QString parentClassName, QString childClassName);
  */
  // ----------------------------------

public:
  QMap<GPdu::Id, GParser*> parsersMap_;
  QVector<GParser*> parserList_;
  virtual bool parse(GPacket* packet);

protected:
  virtual GPdu::Id getNextPduId();
  virtual GPdu* doParse(GPacket* packet);
};
