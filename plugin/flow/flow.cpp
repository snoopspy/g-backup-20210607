#include <GIpFlowMgr>
#include <GTcpFlowMgr>
#include <GUdpFlowMgr>

#ifdef Q_OS_WIN
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

extern "C" {

EXPORT int count() {
	qRegisterMetaType<GIpFlowMgr*>();
	qRegisterMetaType<GTcpFlowMgr*>();
	qRegisterMetaType<GUdpFlowMgr*>();
	return 3;
}

EXPORT const char* name(int index) {
	switch (index) {
		case 0: return GIpFlowMgr::staticMetaObject.className();
		case 1: return GTcpFlowMgr::staticMetaObject.className();
		case 2: return GUdpFlowMgr::staticMetaObject.className();
		default: return nullptr;
	}
}

EXPORT void* create(int index) {
	switch (index) {
		case 0: return new GIpFlowMgr;
		case 1: return new GTcpFlowMgr;
		case 2: return new GUdpFlowMgr;
		default: return nullptr;
	}
}

}
