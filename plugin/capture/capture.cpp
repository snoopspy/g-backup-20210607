#include <GPcapDevice>

extern "C" {

int count() {
  qDebug() << "count()";
  qRegisterMetaType<GPcapDevice*>();
  return 1;
}

char* name(int index) {
  qDebug() << QString("name(%1)").arg(index);
  switch (index) {
    case 0: return (char*)GPcapDevice::staticMetaObject.className();
    default: return nullptr;
  }
}

void* create(int index) {
  qDebug() << QString("...create(%1)").arg(index);
  switch (index) {
    case 0: return new GPcapDevice;
    default: return nullptr;
  }
}

}
