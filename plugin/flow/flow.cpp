#include <GIpFlowMgr>
#include <GUdpFlowMgr>

extern "C" {

int count() {
  qRegisterMetaType<GIpFlowMgr*>();
  qRegisterMetaType<GUdpFlowMgr*>();
  return 2;
}

void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GIpFlowMgr::staticMetaObject;
    case 1: return (void*)&GUdpFlowMgr::staticMetaObject;
    default: return nullptr;
  }
}

void* create(int index) {
  switch (index) {
    case 0: return new GIpFlowMgr;
    case 1: return new GUdpFlowMgr;
    default: return nullptr;
  }
}

}
