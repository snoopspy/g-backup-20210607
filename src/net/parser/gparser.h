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

public:
  GParser* findFirstChild(QString className);
  QVector<GParser*> findAll(QString className);
  void addChild(QString parentClassName, QString childClassName);

public:
  virtual bool parse(GPacket* packet, GPdu* prev = nullptr);

protected:
  virtual GPdu* doParse(GPacket* packet, GPdu* prev);
};
