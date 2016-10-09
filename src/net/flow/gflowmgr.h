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
#include "gflow.h"
#include "gflowkey.h"
#include "net/packet/gpacket.h"

// ----------------------------------------------------------------------------
// GFlowMgr
// ----------------------------------------------------------------------------
struct GFlowMgr : GStateObj {
  Q_OBJECT
  Q_PROPERTY(long checkInterval MEMBER checkInterval_)

public:
  long checkInterval_{1}; // 1 second

public:
  GFlowMgr(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GFlowMgr() override { close(); }

protected:
  bool doOpen() override {
    lastCheckTick_ = 0;
    return true;
  }

  bool doClose() override {
    return true;
  }

public:
  //Snoop_MacFlow_Map   macFlow_Map; // gilgil temp 2016.10.10
  //Snoop_TcpFlow_Map   tcpFlow_Map; // gilgil temp 2016.10.10
  //Snoop_UdpFlow_Map   udpFlow_Map; // gilgil temp 2016.10.10

public:
  //SnoopFlowRequestItems macFlow_Items; // gilgil temp 2016.10.10
  //
  //SnoopFlowRequestItems tcpFlow_Items; // gilgil temp 2016.10.10
  //SnoopFlowRequestItems udpFlow_Items; // gilgil temp 2016.10.10

public:
  // ----- gilgil temp 2016.10.10 -----
  /*
  //
  // MacFlow
  //
  Snoop_MacFlow_Map::iterator add_MacFlow(SnoopMacFlowKey& key, struct timeval ts, bool created);
  Snoop_MacFlow_Map::iterator del_MacFlow(SnoopMacFlowKey& key);

  //
  // IpFlow
  //
  size_t requestMemory_IpFlow(void* id, size_t memSize);
  Snoop_IpFlow_Map::iterator add_IpFlow(SnoopIpFlowKey& key, struct timeval ts, bool created);
  Snoop_IpFlow_Map::iterator del_IpFlow(SnoopIpFlowKey& key);

  //
  // TcpFlow
  //
  size_t requestMemory_TcpFlow(void* id, size_t memSize);
  Snoop_TcpFlow_Map::iterator add_TcpFlow(SnoopTcpFlowKey& key, struct timeval ts, bool created);
  Snoop_TcpFlow_Map::iterator del_TcpFlow(SnoopTcpFlowKey& key);

  //
  // UdpFlow
  //
  size_t requestMemory_UdpFlow(void* id, size_t memSize);
  Snoop_UdpFlow_Map::iterator add_UdpFlow(SnoopUdpFlowKey& key, struct timeval ts, bool created);
  Snoop_UdpFlow_Map::iterator del_UdpFlow(SnoopUdpFlowKey& key);
  */
  // ----------------------------------

protected:
  // ----- gilgil temp 2016.10.10 -----
  /*
  void process_MacFlow(SnoopPacket* packet, SnoopMacFlowKey& key);
  void process_IpFlow(SnoopPacket* packet, SnoopIpFlowKey& key);
  void process_TcpFlow(SnoopPacket* packet, SnoopTcpFlowKey& key);
  void process_UdpFlow(SnoopPacket* packet, SnoopUdpFlowKey& key);
  */
  // ----------------------------------

signals:
  // ----- gilgil temp 2016.10.10 -----
  /*
  //
  // MacFlow
  //
  void __macFlowCreated(SnoopMacFlowKey* key, SnoopFlowValue* value);
  void __macFlowDeleted(SnoopMacFlowKey* key, SnoopFlowValue* value);
  void __macCaptured(SnoopPacket* packet);

  //
  // IpFlow
  //
  void __ipFlowCreated(SnoopIpFlowKey* key, SnoopFlowValue* value);
  void __ipFlowDeleted(SnoopIpFlowKey* key, SnoopFlowValue* value);
  void __ipCaptured(SnoopPacket* packet);

  //
  // TcpFlow
  //
  void __tcpFlowCreated(SnoopTcpFlowKey* key, SnoopFlowValue* value);
  void __tcpFlowDeleted(SnoopTcpFlowKey* key, SnoopFlowValue* value);
  void __tcpCaptured(SnoopPacket* packet);

  //
  // UdpFlow
  //
  void __udpFlowCreated(SnoopUdpFlowKey* key, SnoopFlowValue* value);
  void __udpFlowDeleted(SnoopUdpFlowKey* key, SnoopFlowValue* value);
  void __udpCaptured(SnoopPacket* packet);
  */
  // ----------------------------------

protected:
  long lastCheckTick_{0};

  // ----- gilgil temp 2016.10.10 -----
  /*
  long macFlowTimeout;
  long ipFlowTimeout;
  long tcpFlowTimeout;
  long udpFlowTimeout;
  */
  // ----------------------------------
};
