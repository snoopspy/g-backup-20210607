#include <GIpFlowMgr>
#include <GTcpFlowMgr>
#include <GUdpFlowMgr>

#ifdef Q_OS_WIN
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT
#endif

extern "C" {

EXPORT int count() {
  qRegisterMetaType<GIpFlowMgr*>();
  qRegisterMetaType<GIpFlowMgr*>();
  qRegisterMetaType<GTcpFlowMgr*>();
  return 3;
}

EXPORT void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GIpFlowMgr::staticMetaObject;
    case 1: return (void*)&GTcpFlowMgr::staticMetaObject;
    case 2: return (void*)&GUdpFlowMgr::staticMetaObject;
    default: return nullptr;
  }
}

EXPORT void* create(int index) {
  switch (index) {
    case 0: return new GIpFlowMgr;
    case 1: return new GTcpFlowMgr;
    case 2: return new GUdpFlowMgr;
    default: return nullptr;
  }
}

}
