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

#include "gpcapwrite.h"

// ----------------------------------------------------------------------------
// GPcapFileWrite
// ----------------------------------------------------------------------------
struct G_EXPORT GPcapFileWrite : GPcapWrite {
  Q_OBJECT
  Q_PROPERTY(GPacket::DataLinkType dataLinkType MEMBER dataLinkType_)
  Q_PROPERTY(int snapLen MEMBER snapLen_)
  Q_PROPERTY(QString fileName MEMBER fileName_)
  Q_PROPERTY(bool resolveFileNameByTime MEMBER resolveFileNameByTime_)

public:
  int snapLen_{65536};
  QString fileName_{"pcap/yyyy.MM.dd hh-mm-ss-zzz.'pcap'"};
  bool resolveFileNameByTime_{true};

public:
  Q_INVOKABLE GPcapFileWrite(QObject* parent = nullptr) : GPcapWrite(parent) {
    dataLinkType_ = GPacket::Eth;
  }
  ~GPcapFileWrite() override { close(); }

protected:
  bool doOpen() override;
  bool doClose() override;

protected:
  pcap_dumper_t* pcap_dumper_{nullptr};

public slots:
  void write(GPacket* packet);

signals:
  void written(GPacket* packet);
};
