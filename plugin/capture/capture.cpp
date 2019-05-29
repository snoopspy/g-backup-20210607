#include <GPcapDevice>
#include <GPcapFile>
#include <GNetFilter>
#include <GWinDivert>

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
#endif
#ifdef Q_OS_WIN
  qRegisterMetaType<GWinDivert*>();
#endif
  return 3;
}

EXPORT const char* name(int index) {
  switch (index) {
    case 0: return GPcapDevice::staticMetaObject.className();
    case 1: return GPcapFile::staticMetaObject.className();
#ifdef Q_OS_LINUX
    case 2: return GNetFilter::staticMetaObject.className();
#endif
#ifdef Q_OS_WIN
    case 2: return GWinDivert::staticMetaObject.className();
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
