#include <GPcapDeviceWriter>

extern "C" {

int count() {
  qRegisterMetaType<GPcapDeviceWriter*>();
  return 1;
}

void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GPcapDeviceWriter::staticMetaObject;
    default: return nullptr;
  }
}

void* create(int index) {
  switch (index) {
    case 0: return new GPcapDeviceWriter;
    default: return nullptr;
  }
}

}

