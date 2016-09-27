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
  Q_PROPERTY(int queueNum MEMBER queueNum_)
  Q_PROPERTY(int snapLen MEMBER snapLen_)
  Q_PROPERTY(int bufCnt MEMBER bufCnt_) // gilgil temp 2016.09.27

public:
  int queueNum_{0};
  int snapLen_{65536}; // 65536 bytes
  int bufCnt_{256}; // gilgil temp 2016.09.27

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
  void run() override;

protected:
  struct nfq_handle* h{nullptr};
  struct nfq_q_handle* qh{nullptr};
  int fd{0};

  GParser* parser_{nullptr};

  static int _callback(
    struct nfq_q_handle* qh,
    struct nfgenmsg* nfmsg,
    struct nfq_data* nfad,
    void* data);
};
