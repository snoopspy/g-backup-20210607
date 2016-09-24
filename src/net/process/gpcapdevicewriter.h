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
#include "net/packet/gpacket.h"
#include "gprocess.h"

// ----------------------------------------------------------------------------
// GPcapDeviceWriter
// ----------------------------------------------------------------------------
struct GPcapDeviceWriter : GStateObj {
  Q_OBJECT
  Q_PROPERTY(QString dev MEMBER dev_)

public:
  QString dev_{""};

public:
  Q_INVOKABLE GPcapDeviceWriter(QObject* parent = nullptr);
  ~GPcapDeviceWriter() override;

protected:
  bool doOpen() override;
  bool doClose() override;

public slots:
  GPacket::Result write(GPacket* packet); // gilgil temp 2016.09.24

protected:
  pcap_t* pcap_{nullptr};
};
