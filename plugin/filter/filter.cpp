#include <GBpFilter>

extern "C" {

int count() {
  qRegisterMetaType<GBpFilter*>();
  return 1;
}

void* meta(int index) {
  switch (index) {
    case 0: return (void*)&GBpFilter::staticMetaObject;
    default: return nullptr;
  }
}

void* create(int index) {
  switch (index) {
    case 0: return new GBpFilter;
    default: return nullptr;
  }
}

}
