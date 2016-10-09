#include <GPacketDebugger>
#include "net/process/gipflowmgrtest.h"

extern "C" {

int count() {
  qRegisterMetaType<GPacketDebugger*>();
  qRegisterMetaType<GIpFlowMgrTest*>();
  return 2;
}

void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GPacketDebugger::staticMetaObject;
    case 1: return (void*)&GIpFlowMgrTest::staticMetaObject;
    default: return nullptr;
  }
}

void* create(int index) {
  switch (index) {
    case 0: return new GPacketDebugger;
    case 1: return new GIpFlowMgrTest;
    default: return nullptr;
  }
}

}
