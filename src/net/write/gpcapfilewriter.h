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
// GPcapFileWriter
// ----------------------------------------------------------------------------
struct G_EXPORT GPcapFileWriter : GStateObj {
  Q_OBJECT
  Q_PROPERTY(GPacket::DataLinkType dataLinkType MEMBER dataLinkType_)
  Q_PROPERTY(int snapLen MEMBER snapLen_)
  Q_PROPERTY(QString fileName MEMBER fileName_)

public:
  GPacket::DataLinkType dataLinkType_{GPacket::Eth};
  int snapLen_{65536};
  QString fileName_{"pcap/yyyy.MM.dd hh-mm-ss-zzz.'pcap'"};

public:
  Q_INVOKABLE GPcapFileWriter(QObject* parent = nullptr);
  ~GPcapFileWriter() override;

protected:
  bool doOpen() override;
  bool doClose() override;

public slots:
  void write(GPacket* packet);

signals:
  void written(GPacket* packet);

protected:
  pcap_t* pcap_{nullptr};
  pcap_dumper_t* pcap_dumper_{nullptr};
};
