#include <GPcapDevice>
#include <GPcapFile>
#include <GNetFilter>

extern "C" {

int count() {
  qRegisterMetaType<GNetFilter*>();
  qRegisterMetaType<GPcapDevice*>();
  qRegisterMetaType<GPcapFile*>();
  return 3;
}

void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GNetFilter::staticMetaObject;
    case 1: return (void*)&GPcapDevice::staticMetaObject;
    case 2: return (void*)&GPcapFile::staticMetaObject;
    default: return nullptr;
  }
}

void* create(int index) {
  switch (index) {
    case 0: return new GNetFilter;
    case 1: return new GPcapDevice;
    case 2: return new GPcapFile;
    default: return nullptr;
  }
}

}
