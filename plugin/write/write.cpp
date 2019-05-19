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

EXPORT void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GPcapDeviceWriter::staticMetaObject;
    case 1: return (void*)&GPcapFileWriter::staticMetaObject;
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
