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
    GMac srcMac;
    GMac dstMac;

    bool operator < (const MacFlowKey& rhs) const {
      if (this->srcMac < rhs.srcMac) return true;
      if (this->srcMac > rhs.srcMac) return false;
      if (this->dstMac < rhs.dstMac) return true;
      return false;
    }

    MacFlowKey reverse() {
      MacFlowKey res;
      res.srcMac = this->dstMac;
      res.dstMac = this->srcMac;
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

  // ----------------------------------------------------------------------------
  // IpFlowKey
  // ----------------------------------------------------------------------------
  struct IpFlowKey
  {
  public:
    GIp srcIp;
    GIp dstIp;

    bool operator < (const IpFlowKey& rhs) const {
      if (this->srcIp < rhs.srcIp) return true;
      if (this->srcIp > rhs.srcIp) return false;
      if (this->dstIp < rhs.dstIp) return true;
      return false;
    }

    IpFlowKey reverse() {
      IpFlowKey res;
      res.srcIp = this->dstIp;
      res.dstIp = this->srcIp;
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
    uint16_t srcPort;
    uint16_t dstPort;

    bool operator < (const PortFlowKey& rhs) const {
      if (this->srcPort < rhs.srcPort) return true;
      if (this->srcPort > rhs.srcPort) return false;
      if (this->dstPort < rhs.dstPort) return true;
      return false;
    }

    PortFlowKey reverse() {
      PortFlowKey res;
      res.srcPort = this->dstPort;
      res.dstPort = this->srcPort;
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
    GIp      srcIp;
    uint16_t srcPort;
    GIp      dstIp;
    uint16_t dstPort;

    bool operator < (const TransportFlowKey& rhs) const {
      if (this->srcIp   < rhs.srcIp)   return true;
      if (this->srcIp   > rhs.srcIp)   return false;
      if (this->srcPort < rhs.srcPort) return true;
      if (this->srcPort > rhs.srcPort) return false;
      if (this->dstIp   < rhs.dstIp)   return true;
      if (this->dstIp   > rhs.dstIp)   return false;
      if (this->dstPort < rhs.dstPort) return true;
      return false;
    }

    bool operator == (const TransportFlowKey& rhs) const {
      if (this->srcIp   != rhs.srcIp)   return false;
      if (this->srcIp   != rhs.srcIp)   return false;
      if (this->srcPort != rhs.srcPort) return false;
      if (this->srcPort != rhs.srcPort) return false;
      if (this->dstIp   != rhs.dstIp)   return false;
      if (this->dstIp   != rhs.dstIp)   return false;
      if (this->dstPort != rhs.dstPort) return false;
      return true;
    }

    TransportFlowKey reverse() {
      TransportFlowKey res;
      res.srcIp   = this->dstIp;
      res.srcPort = this->dstPort;
      res.dstIp   = this->srcIp;
      res.dstPort = this->srcPort;
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
