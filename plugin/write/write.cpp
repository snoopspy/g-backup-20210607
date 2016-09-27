#include <GPacketDebugger>
#include <GPcapDeviceWriter>
#include <GPcapFileWriter>

extern "C" {

int count() {
  qRegisterMetaType<GPacketDebugger*>();
  qRegisterMetaType<GPcapDeviceWriter*>();
  qRegisterMetaType<GPcapFileWriter*>();
  return 3;
}

void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GPacketDebugger::staticMetaObject;
    case 1: return (void*)&GPcapDeviceWriter::staticMetaObject;
    case 2: return (void*)&GPcapFileWriter::staticMetaObject;
    default: return nullptr;
  }
}

void* create(int index) {
  switch (index) {
    case 0: return new GPacketDebugger;
    case 1: return new GPcapDeviceWriter;
    case 2: return new GPcapFileWriter;
    default: return nullptr;
  }
}

}
