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
  // MacKey
  // ----------------------------------------------------------------------------
  struct MacKey {
    GMac mac;

    bool operator < (const MacKey& rhs) const {
      return (mac < rhs.mac);
    }
  };

  // ----------------------------------------------------------------------------
  // MacFlowKey
  // ----------------------------------------------------------------------------
  struct MacFlowKey {
    GMac smac;
    GMac dmac;

    bool operator < (const MacFlowKey& rhs) const {
      if (this->smac < rhs.smac) return true;
      if (this->smac > rhs.smac) return false;
      if (this->dmac < rhs.dmac) return true;
      return false;
    }

    MacFlowKey reverse() {
      MacFlowKey res;
      res.smac = this->dmac;
      res.dmac = this->smac;
      return res;
    }
  };

  // ----------------------------------------------------------------------------
  // MacSessionKey
  // ----------------------------------------------------------------------------
  struct MacSessionKey {
    GMac mac1;
    GMac mac2;

    bool operator < (const MacSessionKey& rhs) const {
      if (this->mac1 < rhs.mac1) return true;
      if (this->mac1 > rhs.mac1) return false;
      if (this->mac2 < rhs.mac2) return true;
      return false;
    }
  };

  // ----------------------------------------------------------------------------
  // IpKey
  // ----------------------------------------------------------------------------
  struct IpKey
  {
  public:
    GIp ip;

    bool operator < (const IpKey& rhs) const {
      return (this->ip < rhs.ip);
    }
  };

  // -------------------dstIp---------------------------------------------------------
  // IpFlowKey
  // ----------------------------------------------------------------------------
  struct IpFlowKey
  {
  public:
    GIp sip;
    GIp dip;

    bool operator < (const IpFlowKey& rhs) const {
      if (this->sip < rhs.sip) return true;
      if (this->sip > rhs.sip) return false;
      if (this->dip < rhs.dip) return true;
      return false;
    }

    IpFlowKey reverse() {
      IpFlowKey res;
      res.sip = this->dip;
      res.dip = this->sip;
      return res;
    }
  };

  // ----------------------------------------------------------------------------
  // IpSessionKey
  // ----------------------------------------------------------------------------
  struct IpSessionKey
  {
  public:
    GIp ip1;
    GIp ip2;

    bool operator < (const IpSessionKey& rhs) const {
      if (this->ip1 < rhs.ip1) return true;
      if (this->ip1 > rhs.ip1) return false;
      if (this->ip2 < rhs.ip2) return true;
      return false;
    }
  };

  // ----------------------------------------------------------------------------
  // PortKey
  // ----------------------------------------------------------------------------
  struct PortKey
  {
  public:
    uint16_t port;

    bool operator < (const PortKey& rhs) const {
      return (this->port < rhs.port);
    }
  };

  // ----------------------------------------------------------------------------
  // PortFlowKey
  // ----------------------------------------------------------------------------
  struct PortFlowKey
  {
  public:
    uint16_t sport;
    uint16_t dport;

    bool operator < (const PortFlowKey& rhs) const {
      if (this->sport < rhs.sport) return true;
      if (this->sport > rhs.sport) return false;
      if (this->dport < rhs.dport) return true;
      return false;
    }

    PortFlowKey reverse() {
      PortFlowKey res;
      res.sport = this->dport;
      res.dport = this->sport;
      return res;
    }
  };

  // ----------------------------------------------------------------------------
  // PortSessionKey
  // ----------------------------------------------------------------------------
  struct PortSessionKey
  {
  public:
    uint16_t port1;
    uint16_t port2;

    bool operator < (const PortSessionKey& rhs) const {
      if (this->port1 < rhs.port1) return true;
      if (this->port1 > rhs.port1) return false;
      if (this->port2 < rhs.port2) return true;
      return false;
    }
  };

  // ----------------------------------------------------------------------------
  // TransportKey
  // ----------------------------------------------------------------------------
  struct TransportKey
  {
  public:
    GIp     ip;
    uint16_t port;

    bool operator < (const TransportKey& rhs) const {
      if (this->ip   < rhs.ip) return true;
      if (this->ip   > rhs.ip) return false;
      if (this->port < rhs.port) return true;
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
    GIp      sip;
    uint16_t sport;
    GIp      dip;
    uint16_t dport;

    bool operator < (const TransportFlowKey& rhs) const {
      if (this->sip   < rhs.sip)   return true;
      if (this->sip   > rhs.sip)   return false;
      if (this->sport < rhs.sport) return true;
      if (this->sport > rhs.sport) return false;
      if (this->dip   < rhs.dip)   return true;
      if (this->dip   > rhs.dip)   return false;
      if (this->dport < rhs.dport) return true;
      return false;
    }

    bool operator == (const TransportFlowKey& rhs) const {
      if (this->sip   != rhs.sip)   return false;
      if (this->sip   != rhs.sip)   return false;
      if (this->sport != rhs.sport) return false;
      if (this->sport != rhs.sport) return false;
      if (this->dip   != rhs.dip)   return false;
      if (this->dip   != rhs.dip)   return false;
      if (this->dport != rhs.dport) return false;
      return true;
    }

    TransportFlowKey reverse() {
      TransportFlowKey res;
      res.sip   = this->dip;
      res.sport = this->dport;
      res.dip   = this->sip;
      res.dport = this->sport;
      return res;
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
    GIp      ip1;
    uint16_t port1;
    GIp      ip2;
    uint16_t port2;

    bool operator < (const TransportSessionKey& rhs) const {
      if (this->ip1   < rhs.ip1)   return true;
      if (this->ip1   > rhs.ip1)   return false;
      if (this->port1 < rhs.port1) return true;
      if (this->port1 > rhs.port1) return false;
      if (this->ip2   < rhs.ip2)   return true;
      if (this->ip2   > rhs.ip2)   return false;
      if (this->port2 < rhs.port2) return true;
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
    uint8_t          proto;
    TransportFlowKey flow;

    bool operator < (const TupleFlowKey& rhs) const {
      if (this->proto < rhs.proto) return true;
      if (this->proto > rhs.proto) return false;
      return this->flow < rhs.flow;
    }

    TupleFlowKey reverse() {
      TupleFlowKey res;
      res.proto = this->proto;
      res.flow  = this->flow.reverse();
      return res;
    }
  };
}
