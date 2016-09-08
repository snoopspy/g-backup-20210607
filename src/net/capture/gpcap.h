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
struct GPcap : GCapture {
  Q_OBJECT
  Q_PROPERTY(QString filter MEMBER filter_)

public:
  QString filter_{""};

public:
  enum {
    ERROR_IN_PCAP_NEXT_EX = GErrCategory::PCAP,
    DEVICE_NOT_SPECIFIED
  };

public:
  GPcap(QObject* parent = nullptr) : GCapture(parent) {}
  ~GPcap() override;

protected:
  bool doOpen() override;
  bool doClose() override;

public:
  Result read(GPacket* packet) override;
  Result write(GPacket* packet) override;
  Result write(u_char* buf, size_t len) override;
  Result relay(GPacket* packet) override;

  PathType pathType() override { return OutOfPath; }
  DataLinkType dataLinkType() override { return dataLink_; }

protected:
  bool pcapProcessFilter(pcap_if_t* dev);

  pcap_t*  pcap_{nullptr};
  DataLinkType dataLink_{Null};
};
