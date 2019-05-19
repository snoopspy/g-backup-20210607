#include <GCommand>

#ifdef Q_OS_WIN
  #define EXPORT __declspec(dllexport)
#else
  #define EXPORT
#endif
extern "C" {

EXPORT int count() {
  qRegisterMetaType<GCommand*>();
  return 1;
}

EXPORT void* meta(int index) {
  switch (index) {
    case 0: return pvoid(&GCommand::staticMetaObject);
    default: return nullptr;
  }
}

EXPORT void* create(int index) {
  switch (index) {
    case 0: return new GCommand;
    default: return nullptr;
  }
}

}
