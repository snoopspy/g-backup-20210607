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

#include "net/gip.h"
#include "net/gip6.h"
#include "net/gmac.h"

namespace GFlow {
  // ----------------------------------------------------------------------------
  // MacFlowKey
  // ----------------------------------------------------------------------------
  struct MacFlowKey {
    GMac smac_;
    GMac dmac_;

    MacFlowKey(GMac smac, GMac dmac) : smac_(smac), dmac_(dmac) {}

    bool operator < (const MacFlowKey& rhs) const {
      if (this->smac_ < rhs.smac_) return true;
      if (this->smac_ > rhs.smac_) return false;
      if (this->dmac_ < rhs.dmac_) return true;
      return false;
    }

    MacFlowKey reverse() {
      return MacFlowKey(dmac_, smac_);
    }
  };

  // ----------------------------------------------------------------------------
  // MacSessionKey
  // ----------------------------------------------------------------------------
  struct MacSessionKey {
    GMac mac1_;
    GMac mac2_;

    MacSessionKey(GMac mac1, GMac mac2) : mac1_(mac1), mac2_(mac2) {}

    bool operator < (const MacSessionKey& rhs) const {
      if (this->mac1_ < rhs.mac1_) return true;
      if (this->mac1_ > rhs.mac1_) return false;
      if (this->mac2_ < rhs.mac2_) return true;
      return false;
    }
  };

  // ----------------------------------------------------------------------------
  // IpFlowKey
  // ----------------------------------------------------------------------------
  struct IpFlowKey
  {
  public:
    GIp sip_;
    GIp dip_;

    IpFlowKey(GIp sip, GIp dip) : sip_(sip), dip_(dip) {}

    bool operator < (const IpFlowKey& rhs) const {
      if (this->sip_ < rhs.sip_) return true;
      if (this->sip_ > rhs.sip_) return false;
      if (this->dip_ < rhs.dip_) return true;
      return false;
    }

    IpFlowKey reverse() {
      return IpFlowKey(dip_, sip_);
    }
  };

  // ----------------------------------------------------------------------------
  // IpSessionKey
  // ----------------------------------------------------------------------------
  struct IpSessionKey
  {
  public:
    GIp ip1_;
    GIp ip2_;

    IpSessionKey(GIp ip1, GIp ip2) : ip1_(ip1), ip2_(ip2) {}

    bool operator < (const IpSessionKey& rhs) const {
      if (this->ip1_ < rhs.ip1_) return true;
      if (this->ip1_ > rhs.ip1_) return false;
      if (this->ip2_ < rhs.ip2_) return true;
      return false;
    }
  };

  // ----------------------------------------------------------------------------
  // PortFlowKey
  // ----------------------------------------------------------------------------
  struct PortFlowKey
  {
  public:
    uint16_t sport_;
    uint16_t dport_;

    PortFlowKey(uint16_t sport, uint16_t dport) : sport_(sport), dport_(dport) {}

    bool operator < (const PortFlowKey& rhs) const {
      if (this->sport_ < rhs.sport_) return true;
      if (this->sport_ > rhs.sport_) return false;
      if (this->dport_ < rhs.dport_) return true;
      return false;
    }

    PortFlowKey reverse() {
      return PortFlowKey(dport_, sport_);
    }
  };

  // ----------------------------------------------------------------------------
  // PortSessionKey
  // ----------------------------------------------------------------------------
  struct PortSessionKey
  {
  public:
    uint16_t port1_;
    uint16_t port2_;

    PortSessionKey(uint16_t port1, uint16_t port2) : port1_(port1), port2_(port2) {}

    bool operator < (const PortSessionKey& rhs) const {
      if (this->port1_ < rhs.port1_) return true;
      if (this->port1_ > rhs.port1_) return false;
      if (this->port2_ < rhs.port2_) return true;
      return false;
    }
  };

  // ----------------------------------------------------------------------------
  // TransportKey
  // ----------------------------------------------------------------------------
  struct TransportKey
  {
  public:
    GIp      ip_;
    uint16_t port_;

    TransportKey(GIp ip, uint16_t port) : ip_(ip), port_(port) {}

    bool operator < (const TransportKey& rhs) const {
      if (this->ip_   < rhs.ip_) return true;
      if (this->ip_   > rhs.ip_) return false;
      if (this->port_ < rhs.port_) return true;
      return false;
    }
  };

  typedef TransportKey TcpKey;
  typedef TransportKey UdpKey;

  // ----------------------------------------------------------------------------
  // TransportFlowKey
  // ----------------------------------------------------------------------------
  struct TransportFlowKey
  {
  public:
    GIp      sip_;
    uint16_t sport_;
    GIp      dip_;
    uint16_t dport_;

    TransportFlowKey(GIp sip, uint16_t sport, GIp dip, uint16_t dport) : sip_(sip), sport_(sport), dip_(dip), dport_(dport) {}

    bool operator < (const TransportFlowKey& rhs) const {
      if (this->sip_   < rhs.sip_)   return true;
      if (this->sip_   > rhs.sip_)   return false;
      if (this->sport_ < rhs.sport_) return true;
      if (this->sport_ > rhs.sport_) return false;
      if (this->dip_   < rhs.dip_)   return true;
      if (this->dip_   > rhs.dip_)   return false;
      if (this->dport_ < rhs.dport_) return true;
      return false;
    }

    bool operator == (const TransportFlowKey& rhs) const {
      if (this->sip_   != rhs.sip_)   return false;
      if (this->sip_   != rhs.sip_)   return false;
      if (this->sport_ != rhs.sport_) return false;
      if (this->sport_ != rhs.sport_) return false;
      if (this->dip_   != rhs.dip_)   return false;
      if (this->dip_   != rhs.dip_)   return false;
      if (this->dport_ != rhs.dport_) return false;
      return true;
    }

    TransportFlowKey reverse() {
      return TransportFlowKey(dip_, dport_, sip_, sport_);
    }
  };

  typedef TransportFlowKey TcpFlowKey;
  typedef TransportFlowKey UdpFlowKey;

  // ----------------------------------------------------------------------------
  // TransportSessionKey
  // ----------------------------------------------------------------------------
  struct TransportSessionKey
  {
  public:
    GIp      ip1_;
    uint16_t port1_;
    GIp      ip2_;
    uint16_t port2_;

    TransportSessionKey(GIp ip1, uint16_t port1, GIp ip2, uint16_t port2) : ip1_(ip1), port1_(port1), ip2_(ip2), port2_(port2) {}

    bool operator < (const TransportSessionKey& rhs) const {
      if (this->ip1_   < rhs.ip1_)   return true;
      if (this->ip1_   > rhs.ip1_)   return false;
      if (this->port1_ < rhs.port1_) return true;
      if (this->port1_ > rhs.port1_) return false;
      if (this->ip2_   < rhs.ip2_)   return true;
      if (this->ip2_   > rhs.ip2_)   return false;
      if (this->port2_ < rhs.port2_) return true;
      return false;
    }
  };

  typedef TransportSessionKey TcpSessionKey;
  typedef TransportSessionKey UdpSessionKey;

  // ----------------------------------------------------------------------------
  // TupleFlowKey
  // ----------------------------------------------------------------------------
  struct TupleFlowKey
  {
  public:
    uint8_t          proto_;
    TransportFlowKey flow_;

    TupleFlowKey(uint8_t proto, TransportFlowKey flow) : proto_(proto), flow_(flow) {}

    bool operator < (const TupleFlowKey& rhs) const {
      if (this->proto_ < rhs.proto_) return true;
      if (this->proto_ > rhs.proto_) return false;
      return this->flow_ < rhs.flow_;
    }

    TupleFlowKey reverse() {
      return TupleFlowKey(proto_, flow_.reverse());
    }
  };
}
