#include <GPcapDeviceWriter>
#include <GPcapFileWriter>

#ifdef Q_OS_WIN
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT
#endif

extern "C" {

EXPORT int count() {
  qRegisterMetaType<GPcapDeviceWriter*>();
  qRegisterMetaType<GPcapFileWriter*>();
  return 2;
}

EXPORT const char* name(int index) {
  switch (index) {
    case 0: return GPcapDeviceWriter::staticMetaObject.className();
    case 1: return GPcapFileWriter::staticMetaObject.className();
    default: return nullptr;
  }
}

EXPORT void* create(int index) {
  switch (index) {
    case 0: return new GPcapDeviceWriter;
    case 1: return new GPcapFileWriter;
    default: return nullptr;
  }
}

}
