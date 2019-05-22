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

EXPORT const char* name(int index) {
  switch (index) {
    case 0: return GPcapDevice::staticMetaObject.className();
    case 1: return GPcapFile::staticMetaObject.className();
#ifdef Q_OS_LINUX
    case 2: return GNetFilter::staticMetaObject.className();
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
