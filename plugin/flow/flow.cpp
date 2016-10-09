#include <GIpFlowMgr>

extern "C" {

int count() {
  qRegisterMetaType<GIpFlowMgr*>();
  return 1;
}

void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GIpFlowMgr::staticMetaObject;
    default: return nullptr;
  }
}

void* create(int index) {
  switch (index) {
    case 0: return new GIpFlowMgr;
    default: return nullptr;
  }
}

}
