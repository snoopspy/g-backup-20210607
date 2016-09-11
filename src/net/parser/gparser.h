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

#include "base/gobj.h"

// ----------------------------------------------------------------------------
// GParsers
// ----------------------------------------------------------------------------
struct GParser;
typedef QVector<GParser*> GParsers;

// ----------------------------------------------------------------------------
// GParser
// ----------------------------------------------------------------------------
struct GPacket;
struct GPdu;
struct GParser : GObj {
  Q_OBJECT

public:
  GParser(QObject* parent = nullptr) : GObj(parent) {}
  ~GParser() override {}

  virtual size_t parse(GPacket* packet);

public:
  GParser* findFirstChild(QString className);
  QVector<GParser*> findAll(QString className);
  void addChild(QString myClassName, QString childClassName);

protected:
  virtual bool isMatch(GPdu* prev, GPacket* packet);
  virtual GPdu* doParse(GPacket* packet);
};
