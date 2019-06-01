#include <GPcapDeviceWrite>
#include <GPcapDeviceWriteEth>
#include <GPcapDeviceWriteEthAutoMac>
#include <GPcapDeviceWriteIp>
#include <GPcapFileWrite>
#include <GPcapFileWriteEth>
#include <GPcapFileWriteIp>

#ifdef Q_OS_WIN
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" {

EXPORT int count() {
  qRegisterMetaType<GPcapDeviceWrite*>();
  qRegisterMetaType<GPcapDeviceWriteEth*>();
  qRegisterMetaType<GPcapDeviceWriteEthAutoMac*>();
  qRegisterMetaType<GPcapDeviceWriteIp*>();
  qRegisterMetaType<GPcapFileWrite*>();
  qRegisterMetaType<GPcapFileWriteEth*>();
  qRegisterMetaType<GPcapFileWriteIp*>();
  return 7;
}

EXPORT const char* name(int index) {
  switch (index) {
    case 0: return GPcapDeviceWrite::staticMetaObject.className();
    case 1: return GPcapDeviceWriteEth::staticMetaObject.className();
    case 2: return GPcapDeviceWriteEthAutoMac::staticMetaObject.className();
    case 3: return GPcapDeviceWriteIp::staticMetaObject.className();
    case 4: return GPcapFileWrite::staticMetaObject.className();
    case 5: return GPcapFileWriteEth::staticMetaObject.className();
    case 6: return GPcapFileWriteIp::staticMetaObject.className();
    default: return nullptr;
  }
}

EXPORT void* create(int index) {
  switch (index) {
    case 0: return new GPcapDeviceWrite;
    case 1: return new GPcapDeviceWriteEth;
    case 2: return new GPcapDeviceWriteEthAutoMac;
    case 3: return new GPcapDeviceWriteIp;
    case 4: return new GPcapFileWrite;
    case 5: return new GPcapFileWriteEth;
    case 6: return new GPcapFileWriteIp;
    default: return nullptr;
  }
}

}
