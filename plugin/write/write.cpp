#include <GPcapDeviceWrite>
#include <GPcapFileWrite>

#ifdef Q_OS_WIN
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT
#endif

extern "C" {

EXPORT int count() {
  qRegisterMetaType<GPcapDeviceWrite*>();
  qRegisterMetaType<GPcapFileWrite*>();
  return 2;
}

EXPORT const char* name(int index) {
  switch (index) {
    case 0: return GPcapDeviceWrite::staticMetaObject.className();
    case 1: return GPcapFileWrite::staticMetaObject.className();
    default: return nullptr;
  }
}

EXPORT void* create(int index) {
  switch (index) {
    case 0: return new GPcapDeviceWrite;
    case 1: return new GPcapFileWrite;
    default: return nullptr;
  }
}

}
