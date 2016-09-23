#include <GPcapDevice>
#include <GPcapFile>

extern "C" {

int count() {
  qDebug() << "count()";
  qRegisterMetaType<GPcapDevice*>();
  qRegisterMetaType<GPcapFile*>();
  return 2;
}

char* name(int index) {
  qDebug() << QString("name(%1)").arg(index);
  switch (index) {
    case 0: return (char*)GPcapDevice::staticMetaObject.className();
    case 1: return (char*)GPcapFile::staticMetaObject.className();
    default: return nullptr;
  }
}

void* create(int index) {
  qDebug() << QString("...create(%1)").arg(index);
  switch (index) {
    case 0: return new GPcapDevice;
    case 1: return new GPcapFile;
    default: return nullptr;
  }
}

}
