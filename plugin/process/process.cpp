#include <GPcapDeviceWriter>

extern "C" {

int count() {
  qDebug() << "count()";
  qRegisterMetaType<GPcapDeviceWriter*>();
  return 1;
}

char* name(int index) {
  qDebug() << QString("name(%1)").arg(index);
  switch (index) {
    case 0: return (char*)GPcapDeviceWriter::staticMetaObject.className();
    default: return nullptr;
  }
}

void* create(int index) {
  qDebug() << QString("...create(%1)").arg(index);
  switch (index) {
    case 0: return new GPcapDeviceWriter;
    default: return nullptr;
  }
}

}

