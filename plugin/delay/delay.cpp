#include <GDelay>
#include <GSyncDelay>
#include <GTimeStampSyncDelay>

#ifdef Q_OS_WIN
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" {

EXPORT int count() {
	qRegisterMetaType<GDelay*>();
	qRegisterMetaType<GSyncDelay*>();
	qRegisterMetaType<GTimeStampSyncDelay*>();
	return 3;
}

EXPORT const char* name(int index) {
	switch (index) {
		case 0: return GDelay::staticMetaObject.className();
		case 1: return GSyncDelay::staticMetaObject.className();
		case 2: return GTimeStampSyncDelay::staticMetaObject.className();
		default: return nullptr;
	}
}

EXPORT void* create(int index) {
	switch (index) {
		case 0: return new GDelay;
		case 1: return new GSyncDelay;
		case 2: return new GTimeStampSyncDelay;
		default: return nullptr;
	}
}

}
