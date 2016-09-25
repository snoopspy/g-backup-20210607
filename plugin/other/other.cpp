#include <GCommand>

extern "C" {

int count() {
  qRegisterMetaType<GCommand*>();
  return 1;
}

void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GCommand::staticMetaObject;
    default: return nullptr;
  }
}

void* create(int index) {
  switch (index) {
    case 0: return new GCommand;
    default: return nullptr;
  }
}

}
