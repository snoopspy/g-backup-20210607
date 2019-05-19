#include <GPcapDevice>
#include <GPcapFile>
#include <GNetFilter>

#ifdef Q_OS_WIN
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT
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
    case 0: return pvoid(&GPcapDevice::staticMetaObject);
    case 1: return pvoid(&GPcapFile::staticMetaObject);
#ifdef Q_OS_LINUX
    case 2: return pvoid(&GNetFilter::staticMetaObject);
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
