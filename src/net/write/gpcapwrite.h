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

#include "base/gstateobj.h"
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GPcapWrite
// ----------------------------------------------------------------------------
struct G_EXPORT GPcapWrite : GStateObj {

public:
  Q_INVOKABLE GPcapWrite(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GPcapWrite() override {}

  GPacket::DataLinkType dataLinkType() { return dataLinkType_; }

protected:
  pcap_t* pcap_{nullptr};
  GPacket::DataLinkType dataLinkType_{GPacket::Null};
};
