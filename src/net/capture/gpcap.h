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

#include <pcap.h>
#include "gcapture.h"

// ----------------------------------------------------------------------------
// GPcap
// ----------------------------------------------------------------------------
struct G_EXPORT GPcap : GCapture {
  Q_OBJECT
  Q_PROPERTY(QString filter MEMBER filter_)

public:
  QString filter_{""};

public:
  GPcap(QObject* parent = nullptr) : GCapture(parent) {}
  ~GPcap() override { close(); }

protected:
  bool doOpen() override;
  bool doClose() override;

public:
  GPacket::Result read(GPacket* packet) override;
  GPacket::Result write(GPacket* packet) override;
  GPacket::Result write(GBuf buf) override;
  GPacket::Result relay(GPacket* packet) override;

  GPacket::DataLinkType dataLinkType() override { return dataLinkType_; }
  PathType pathType() override { return OutOfPath; }

protected:
  bool pcapProcessFilter(pcap_if_t* dev);

  pcap_t* pcap_{nullptr};
  GPacket::DataLinkType dataLinkType_{GPacket::Null};
};
