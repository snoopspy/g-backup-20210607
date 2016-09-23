#include <GPcapDevice>

extern "C" {

int count() {
  return 1;
}

char* name(int index) {
  switch (index) {
    case 0: return (char*)GPcapDevice::staticMetaObject.className();
    default: return nullptr;
  }
}

void* create(int index) {
  switch (index) {
    case 0: return new GPcapDevice;
    default: return nullptr;
  }
}

}
