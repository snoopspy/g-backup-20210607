#include <GSleep>
#include <GDelay>
#include <GTimeStampDelay>

#ifdef Q_OS_WIN
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT
#endif

extern "C" {

EXPORT int count() {
  qRegisterMetaType<GDelay*>();
  qRegisterMetaType<GSleep*>();
  qRegisterMetaType<GTimeStampDelay*>();
  return 3;
}

EXPORT void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GDelay::staticMetaObject;
    case 1: return (void*)&GSleep::staticMetaObject;
    case 2: return (void*)&GTimeStampDelay::staticMetaObject;
    default: return nullptr;
  }
}

EXPORT void* create(int index) {
  switch (index) {
    case 0: return new GDelay;
    case 1: return new GSleep;
    case 2: return new GTimeStampDelay;
    default: return nullptr;
  }
}

}
