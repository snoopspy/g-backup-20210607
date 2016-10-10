#include <GPacketDebugger>
#include "net/process/gflowmgrtest.h"

extern "C" {

int count() {
  qRegisterMetaType<GPacketDebugger*>();
  qRegisterMetaType<GFlowMgrTest*>();
  return 2;
}

void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GPacketDebugger::staticMetaObject;
    case 1: return (void*)&GFlowMgrTest::staticMetaObject;
    default: return nullptr;
  }
}

void* create(int index) {
  switch (index) {
    case 0: return new GPacketDebugger;
    case 1: return new GFlowMgrTest;
    default: return nullptr;
  }
}

}
