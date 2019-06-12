#include <GBpFilter>

#ifdef Q_OS_WIN
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" {

EXPORT int count() {
	qRegisterMetaType<GBpFilter*>();
	return 1;
}

EXPORT const char* name(int index) {
	switch (index) {
		case 0: return GBpFilter::staticMetaObject.className();
		default: return nullptr;
	}
}

EXPORT void* create(int index) {
	switch (index) {
		case 0: return new GBpFilter;
		default: return nullptr;
	}
}

}
