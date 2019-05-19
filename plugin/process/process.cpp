#include <GDnsFirewall>
#include <GDnsProcessor>
#include <GPacketDebugger>
#include "net/process/gflowmgrtest.h"

#ifdef Q_OS_WIN
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT
#endif
extern "C" {

EXPORT int count() {
  qRegisterMetaType<GDnsFirewall*>();
  qRegisterMetaType<GDnsProcessor*>();
  qRegisterMetaType<GPacketDebugger*>();
  qRegisterMetaType<GFlowMgrTest*>();
  return 4;
}

EXPORT void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GDnsFirewall::staticMetaObject;
    case 1: return (void*)&GDnsProcessor::staticMetaObject;
    case 2: return (void*)&GPacketDebugger::staticMetaObject;
    case 3: return (void*)&GFlowMgrTest::staticMetaObject;
    default: return nullptr;
  }
}

EXPORT void* create(int index) {
  switch (index) {
    case 0: return new GDnsFirewall;
    case 1: return new GDnsProcessor;
    case 2: return new GPacketDebugger;
    case 3: return new GFlowMgrTest;
    default: return nullptr;
  }
}

}
