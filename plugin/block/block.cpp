#include <GBlock>

extern "C" {

int count() {
  qRegisterMetaType<GBlock*>();
  return 1;
}

void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GBlock::staticMetaObject;
    default: return nullptr;
  }
}

void* create(int index) {
  switch (index) {
    case 0: return new GBlock;
    default: return nullptr;
  }
}

}
