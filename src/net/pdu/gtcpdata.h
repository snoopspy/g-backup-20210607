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
  static const GPduType staticType = GPduType::TcpData;
  GPduType pduType() override { return staticType; }
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
struct GTcpDataParser : GParser {
  Q_OBJECT

public:
  Q_INVOKABLE GTcpDataParser(QObject* parent = nullptr) : GParser(parent) {}

protected:
  GPdu* doParse(GPacket* packet, GPdu* prev) override;
};
