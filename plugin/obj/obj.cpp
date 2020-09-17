#include "myobj.h"

#ifdef Q_OS_WIN
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" {

EXPORT int count() {
	qRegisterMetaType<GMyObjA*>();
	qRegisterMetaType<GMyObjB*>();
	qRegisterMetaType<GMyObjC*>();
	return 3;
}

EXPORT const char* name(int index) {
	switch (index) {
		case 0: return GMyObjA::staticMetaObject.className();
		case 1: return GMyObjB::staticMetaObject.className();
		case 2: return GMyObjC::staticMetaObject.className();
		default: return nullptr;
	}
}

EXPORT void* create(int index) {
	switch (index) {
		case 0: return new GMyObjA;
		case 1: return new GMyObjB;
		case 2: return new GMyObjC;
		default: return nullptr;
	}
}

}
