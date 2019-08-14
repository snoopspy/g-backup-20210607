#include <GArpSpoof>
#include <GPcapDevice>
#include <GPcapFile>
#include <GNetFilter>
#include <GWinDivert>

#ifdef Q_OS_WIN
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" {

EXPORT int count() {
  qRegisterMetaType<GArpSpoof*>();
  qRegisterMetaType<GPcapDevice*>();
	qRegisterMetaType<GPcapFile*>();
#ifdef Q_OS_LINUX
	qRegisterMetaType<GNetFilter*>();
#endif
#ifdef Q_OS_WIN
	qRegisterMetaType<GWinDivert*>();
#endif
  return 4;
}

EXPORT const char* name(int index) {
	switch (index) {
    case 0: return GArpSpoof::staticMetaObject.className();
    case 1: return GPcapDevice::staticMetaObject.className();
    case 2: return GPcapFile::staticMetaObject.className();
#ifdef Q_OS_LINUX
    case 3: return GNetFilter::staticMetaObject.className();
#endif
#ifdef Q_OS_WIN
    case 3: return GWinDivert::staticMetaObject.className();
#endif
		default: return nullptr;
	}
}

EXPORT void* create(int index) {
	switch (index) {
    case 0: return new GArpSpoof;
    case 1: return new GPcapDevice;
    case 2: return new GPcapFile;
#ifdef Q_OS_LINUX
    case 3: return new GNetFilter;
#endif
#ifdef Q_OS_WIN
    case 3: return new GWinDivert;
#endif
    default: return nullptr;
	}
}

}
