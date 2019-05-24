#include <GDnsFirewall>
#include <GDnsProcessor>
#include <GFlowMgrDebugger>
#include <GPacketDebugger>

#ifdef Q_OS_WIN
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT
#endif
extern "C" {

EXPORT int count() {
  qRegisterMetaType<GDnsFirewall*>();
  qRegisterMetaType<GDnsProcessor*>();
  qRegisterMetaType<GFlowMgrDebugger*>();
  qRegisterMetaType<GPacketDebugger*>();
  return 4;
}

EXPORT const char* name(int index) {
  switch (index) {
    case 0: return GDnsFirewall::staticMetaObject.className();
    case 1: return GDnsProcessor::staticMetaObject.className();
    case 2: return GFlowMgrDebugger::staticMetaObject.className();
    case 3: return GPacketDebugger::staticMetaObject.className();
    default: return nullptr;
  }
}

EXPORT void* create(int index) {
  switch (index) {
    case 0: return new GDnsFirewall;
    case 1: return new GDnsProcessor;
    case 2: return new GFlowMgrDebugger;
    case 3: return new GPacketDebugger;
    default: return nullptr;
  }
}

}
