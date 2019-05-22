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

EXPORT const char* name(int index) {
  switch (index) {
    case 0: return GDelay::staticMetaObject.className();
    case 1: return GSleep::staticMetaObject.className();
    case 2: return GTimeStampDelay::staticMetaObject.className();
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
