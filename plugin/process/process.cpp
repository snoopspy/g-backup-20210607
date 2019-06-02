#include <GDnsProcessor>
#include <GFlowMgrDebugger>
#include <GPacketDebugger>
#include <GBypassSslBlock>

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
  qRegisterMetaType<GBypassSslBlock*>();
  return 4;
}

EXPORT const char* name(int index) {
  switch (index) {
    case 0: return GDnsProcessor::staticMetaObject.className();
    case 1: return GFlowMgrDebugger::staticMetaObject.className();
    case 2: return GPacketDebugger::staticMetaObject.className();
    case 3: return GBypassSslBlock::staticMetaObject.className();
    default: return nullptr;
  }
}

EXPORT void* create(int index) {
  switch (index) {
    case 0: return new GDnsProcessor;
    case 1: return new GFlowMgrDebugger;
    case 2: return new GPacketDebugger;
    case 3: return new GBypassSslBlock;
    default: return nullptr;
  }
}

}

