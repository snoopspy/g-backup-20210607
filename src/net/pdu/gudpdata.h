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
// GUdpData
// ----------------------------------------------------------------------------
struct GUdpData : GPdu {
  static const GPduType staticType = GPduType::UdpData;
  GPduType pduType() override { return staticType; }
  size_t size() override;

  GUdpData(u_char* data, size_t size);

  u_char* data() { return data_; }

protected:
  u_char* data_;
  size_t size_;
};

// ----------------------------------------------------------------------------
// GUdpDataParser
// ----------------------------------------------------------------------------
struct GUdpDataParser : GParser {
  Q_OBJECT

public:
  Q_INVOKABLE GUdpDataParser(QObject* parent = nullptr) : GParser(parent) {}

protected:
  GPdu* doParse(GPacket* packet, GPdu* prev) override;
};
