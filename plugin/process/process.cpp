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
  qRegisterMetaType<GDnsProcessor*>();
  qRegisterMetaType<GFlowMgrDebugger*>();
  qRegisterMetaType<GPacketDebugger*>();
  return 3;
}

EXPORT const char* name(int index) {
  switch (index) {
    case 0: return GDnsProcessor::staticMetaObject.className();
    case 1: return GFlowMgrDebugger::staticMetaObject.className();
    case 2: return GPacketDebugger::staticMetaObject.className();
    default: return nullptr;
  }
}

EXPORT void* create(int index) {
  switch (index) {
    case 0: return new GDnsProcessor;
    case 1: return new GFlowMgrDebugger;
    case 2: return new GPacketDebugger;
    default: return nullptr;
  }
}

}
