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

#include "gcapture.h"

// ----------------------------------------------------------------------------
// GNetFilter
// ----------------------------------------------------------------------------
struct GNetFilter : GCapture {
  Q_OBJECT
  Q_PROPERTY(int num MEMBER num_)
  Q_PROPERTY(int snapLen MEMBER snapLen_)

public:
  int num_{0};
  int snapLen_{65536}; // 65536 bytes

public:
  Q_INVOKABLE GNetFilter(QObject* parent = nullptr);
  ~GNetFilter() override;

protected:
  bool doOpen() override;
  bool doClose() override;

public:
  GPacket::Result read(GPacket* packet) override;
  GPacket::Result write(GPacket* packet) override;
  GPacket::Result write(u_char* buf, size_t len) override;
  GPacket::Result relay(GPacket* packet) override;

  GPacket::DataLinkType dataLinkType() override { return GPacket::Ipv4; }
  PathType pathType() override { return InPath; }

protected:
  struct nfq_handle *h;
};
