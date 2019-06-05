#include <GBypassSslBlock>
#include <GCorrectChecksum>
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
  qRegisterMetaType<GBypassSslBlock*>();
  qRegisterMetaType<GCorrectChecksum*>();
  qRegisterMetaType<GDnsProcessor*>();
  qRegisterMetaType<GFlowMgrDebugger*>();
  qRegisterMetaType<GPacketDebugger*>();
  return 5;
}

EXPORT const char* name(int index) {
  switch (index) {
    case 0: return GBypassSslBlock::staticMetaObject.className();
    case 1: return GCorrectChecksum::staticMetaObject.className();
    case 2: return GDnsProcessor::staticMetaObject.className();
    case 3: return GFlowMgrDebugger::staticMetaObject.className();
    case 4: return GPacketDebugger::staticMetaObject.className();
    default: return nullptr;
  }
}

EXPORT void* create(int index) {
  switch (index) {
    case 0: return new GBypassSslBlock;
    case 1: return new GCorrectChecksum;
    case 2: return new GDnsProcessor;
    case 3: return new GFlowMgrDebugger;
    case 4: return new GPacketDebugger;
    default: return nullptr;
  }
}

}

