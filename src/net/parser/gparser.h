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
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GParser
// ----------------------------------------------------------------------------
struct G_EXPORT GParser : GObj {
  Q_OBJECT

public:
  GParser(QObject* parent = nullptr) : GObj(parent) {}
  ~GParser() override {}

public:
  virtual void parse(GPacket* packet);
};

// ----- gilgil temp 2019.05.13 -----
/*
// ----------------------------------------------------------------------------
// GParsers
// ----------------------------------------------------------------------------
typedef QVector<GParser*> GParsers;
*/
// ----------------------------------
