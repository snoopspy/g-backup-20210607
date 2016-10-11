#include "net/process/gatekdnsfirewall.h"
#include <GDnsProcessor>
#include <GPacketDebugger>
#include "net/process/gflowmgrtest.h"

extern "C" {

int count() {
  qRegisterMetaType<GAtekDnsFirewall*>();
  qRegisterMetaType<GDnsProcessor*>();
  qRegisterMetaType<GPacketDebugger*>();
  qRegisterMetaType<GFlowMgrTest*>();
  return 4;
}

void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GAtekDnsFirewall::staticMetaObject;
    case 1: return (void*)&GDnsProcessor::staticMetaObject;
    case 2: return (void*)&GPacketDebugger::staticMetaObject;
    case 3: return (void*)&GFlowMgrTest::staticMetaObject;
    default: return nullptr;
  }
}

void* create(int index) {
  switch (index) {
    case 0: return new GAtekDnsFirewall;
    case 1: return new GDnsProcessor;
    case 2: return new GPacketDebugger;
    case 3: return new GFlowMgrTest;
    default: return nullptr;
  }
}

}
