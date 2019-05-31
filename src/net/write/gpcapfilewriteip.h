// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#include "gpcapfilewrite.h"

// ----------------------------------------------------------------------------
// GPcapFileWriteIp
// ----------------------------------------------------------------------------
struct G_EXPORT GPcapFileWriteIp : GPcapFileWrite {
  Q_OBJECT

public:
  Q_INVOKABLE GPcapFileWriteIp(QObject* parent = nullptr) : GPcapFileWrite(parent) {
    dataLinkType_ = GPacket::Ip;
  }
  ~GPcapFileWriteIp() override { close(); }

protected:
  bool doOpen() override;
  bool doClose() override;

public slots:
  GPacket::Result write(GPacket* packet);
};
