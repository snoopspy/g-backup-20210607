#include <GPcapDevice>
#include <GPcapFile>
#include <GNetFilter>

#ifdef Q_OS_WIN
  #define EXPORT __declspec(dllexport)
#endif

extern "C" {

EXPORT int count() {
  qRegisterMetaType<GPcapDevice*>();
  qRegisterMetaType<GPcapFile*>();
#ifdef Q_OS_LINUX
  qRegisterMetaType<GNetFilter*>();
  return 3;
#endif
#ifdef Q_OS_WIN
  return 2;
#endif
}

EXPORT void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GPcapDevice::staticMetaObject;
    case 1: return (void*)&GPcapFile::staticMetaObject;
#ifdef Q_OS_LINUX
    case 2: return (void*)&GNetFilter::staticMetaObject;
#endif
    default: return nullptr;
  }
}

EXPORT void* create(int index) {
  switch (index) {
    case 0: return new GPcapDevice;
    case 1: return new GPcapFile;
#ifdef Q_OS_LINUX
    case 2: return new GNetFilter;
#endif
    default: return nullptr;
  }
}

}
