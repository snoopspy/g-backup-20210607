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

#include "gpdu.h"

// ----------------------------------------------------------------------------
// GTcpData
// ----------------------------------------------------------------------------
struct GTcpData : GPdu {
  static const GPdu::Type staticType = GPdu::Type::TcpData;
  GPdu::Type pduType() override { return staticType; }
  GPdu::Id nextPduId() override { return NULL_PDU_ID; }
  size_t size() override;

  GTcpData(u_char* data, size_t size);

  u_char* data() { return data_; }

protected:
  u_char* data_;
  size_t size_;
};

// ----------------------------------------------------------------------------
// GTcpDataParser
// ----------------------------------------------------------------------------
#include "net/parser/gparser.h"
struct GTcpDataParser : GParser {
  Q_OBJECT

public:
  Q_INVOKABLE GTcpDataParser(QObject* parent = nullptr) : GParser(parent) {}

protected:
  GPdu* doParse(GPacket* packet) override;
};
