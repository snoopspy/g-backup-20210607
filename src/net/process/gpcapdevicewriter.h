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
struct GPcapDeviceWriter : GProcess {
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
  GPacket::Result write(GPacket* packet);

protected:
  pcap_t* pcap_{nullptr};

#ifdef QT_GUI_LIB
  GPropItem* propCreateItem(GPropItemParam param) override;
#endif // QT_GUI_LIB
};
