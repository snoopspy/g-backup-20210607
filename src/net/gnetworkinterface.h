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
#include <QList>
#include "net/gip.h"
#include "net/gmac.h"

// ----------------------------------------------------------------------------
// GNetworkInterface
// ----------------------------------------------------------------------------
struct GNetworkInterfaces;
struct GNetworkInterface {
  int index_{-1};
  QString name_;
  QString description_;
  pcap_if_t* dev_{nullptr};
  GMac mac_{GMac::cleanMac()};
  GIp ip_{(quint32)0};
  GIp subnet_{(quint32)0};
  GIp gateway_{(quint32)0};

protected:
  GIp ip_and_subnet_; // used for isSameLanIP

public:
  bool isSameLanIp(GIp ip) { return (ip_and_subnet_) == (ip & subnet_); }
  GIp  getAdjIp(GIp ip)    { return isSameLanIp(ip) ? ip : gateway_;    }
  GIp  getStartIp()        { return (ip_ & subnet_) + 1;                }
  GIp  getEndIp()          { return (ip_ | ~subnet_);                   }

public:
  static GNetworkInterfaces& allInterfaces();
};

// ----------------------------------------------------------------------------
// GNetworkInterfaces
// ----------------------------------------------------------------------------
struct GNetworkInterfaces : QList<GNetworkInterface> {
public: // gilgil temp 2015.11.08
  GNetworkInterfaces();
  virtual ~GNetworkInterfaces();
  pcap_if_t* allDevs_{nullptr};
  static GNetworkInterfaces& instance();
};
