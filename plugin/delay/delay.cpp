#include <GSleep>
#include <GDelay>

#ifdef Q_OS_WIN
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT
#endif

extern "C" {

EXPORT int count() {
  qRegisterMetaType<GDelay*>();
  qRegisterMetaType<GSleep*>();
  return 2;
}

EXPORT void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GDelay::staticMetaObject;
    case 1: return (void*)&GSleep::staticMetaObject;
    default: return nullptr;
  }
}

EXPORT void* create(int index) {
  switch (index) {
    case 0: return new GDelay;
    case 1: return new GSleep;
    default: return nullptr;
  }
}

}
