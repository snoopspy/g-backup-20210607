#include <GPacketDebugger>

extern "C" {

int count() {
  qRegisterMetaType<GPacketDebugger*>();
  return 3;
}

void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GPacketDebugger::staticMetaObject;
    default: return nullptr;
  }
}

void* create(int index) {
  switch (index) {
    case 0: return new GPacketDebugger;
    default: return nullptr;
  }
}

}
