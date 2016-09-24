#include <GPcapDevice>
#include <GPcapFile>

extern "C" {

int count() {
  qRegisterMetaType<GPcapDevice*>();
  qRegisterMetaType<GPcapFile*>();
  return 2;
}

void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GPcapDevice::staticMetaObject;
    case 1: return (void*)&GPcapFile::staticMetaObject;
    default: return nullptr;
  }
}

void* create(int index) {
  switch (index) {
    case 0: return new GPcapDevice;
    case 1: return new GPcapFile;
    default: return nullptr;
  }
}

}
