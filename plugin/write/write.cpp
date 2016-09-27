#include <GPcapDeviceWriter>
#include <GPcapFileWriter>

extern "C" {

int count() {
  qRegisterMetaType<GPcapDeviceWriter*>();
  qRegisterMetaType<GPcapFileWriter*>();
  return 2;
}

void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GPcapDeviceWriter::staticMetaObject;
    case 1: return (void*)&GPcapFileWriter::staticMetaObject;
    default: return nullptr;
  }
}

void* create(int index) {
  switch (index) {
    case 0: return new GPcapDeviceWriter;
    case 1: return new GPcapFileWriter;
    default: return nullptr;
  }
}

}
