#include <GDnsProcessor>
#include <GPacketDebugger>
#include "net/process/gflowmgrtest.h"

extern "C" {

int count() {
  qRegisterMetaType<GDnsProcessor*>();
  qRegisterMetaType<GPacketDebugger*>();
  qRegisterMetaType<GFlowMgrTest*>();
  return 3;
}

void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GDnsProcessor::staticMetaObject;
    case 1: return (void*)&GPacketDebugger::staticMetaObject;
    case 2: return (void*)&GFlowMgrTest::staticMetaObject;
    default: return nullptr;
  }
}

void* create(int index) {
  switch (index) {
    case 0: return new GDnsProcessor;
    case 1: return new GPacketDebugger;
    case 2: return new GFlowMgrTest;
    default: return nullptr;
  }
}

}
