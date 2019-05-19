#include <GBlock>

#ifdef Q_OS_WIN
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT
#endif

extern "C" {

EXPORT int count() {
  qRegisterMetaType<GBlock*>();
  return 1;
}

EXPORT void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GBlock::staticMetaObject;
    default: return nullptr;
  }
}

EXPORT void* create(int index) {
  switch (index) {
    case 0: return new GBlock;
    default: return nullptr;
  }
}

}
