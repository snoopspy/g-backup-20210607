#include <GBpFilter>

#ifdef Q_OS_WIN
  #define EXPORT __declspec(dllexport)
#endif

extern "C" {

EXPORT int count() {
  qRegisterMetaType<GBpFilter*>();
  return 1;
}

EXPORT void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GBpFilter::staticMetaObject;
    default: return nullptr;
  }
}

EXPORT void* create(int index) {
  switch (index) {
    case 0: return new GBpFilter;
    default: return nullptr;
  }
}

}
