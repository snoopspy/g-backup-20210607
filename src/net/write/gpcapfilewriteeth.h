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
// GPcapFileWriteEth
// ----------------------------------------------------------------------------
struct G_EXPORT GPcapFileWriteEth : GPcapFileWrite {
  Q_OBJECT
  Q_PROPERTY(QString smac MEMBER smacStr_)
  Q_PROPERTY(QString dmac MEMBER dmacStr_)
  Q_PROPERTY(quint16 type MEMBER type_)

public:
  QString smacStr_{"000000-000000"};
  QString dmacStr_{"000000-000000"};
  uint16_t type_{GEthHdr::Ip4};

public:
  Q_INVOKABLE GPcapFileWriteEth(QObject* parent = nullptr) : GPcapFileWrite(parent) {
    dataLinkType_ = GPacket::Eth;
  }
  ~GPcapFileWriteEth() override { close(); }

protected:
  bool doOpen() override;
  bool doClose() override;

protected:
  GMac smac_;
  GMac dmac_;
  static const int MAXBUF = 65536;
  gbyte temp_[MAXBUF];

public slots:
  GPacket::Result write(GPacket* packet);
};
