#include "gpcapdevice.h"
#include "net/gnetinfo.h"

// ----------------------------------------------------------------------------
// GPcapDevice
// ----------------------------------------------------------------------------
GPcapDevice::GPcapDevice(QObject* parent) : GPcap(parent) {
	GRtmEntry* entry = GNetInfo::instance().rtm().getBestEntry(QString("8.8.8.8"));
	if (entry != nullptr)
		intfName_ = entry->intf()->name();
}

GPcapDevice::~GPcapDevice() {
	close();
}

bool GPcapDevice::doOpen() {
	if (!enabled_) return true;

	if (intfName_ == "") {
		SET_ERR(GErr::DEVICE_NOT_SPECIFIED, "device is not specified");
		return false;
	}

	intf_ = GNetInfo::instance().interfaceList().findByName(intfName_);
	if (intf_ == nullptr) {
		QString msg = QString("can not find interface for %1").arg(intfName_);
		SET_ERR(GErr::VALUE_IS_NULL, msg);
		return false;
	}

	char errBuf[PCAP_ERRBUF_SIZE];
	pcap_ = pcap_open_live(qPrintable(intfName_), snapLen_, flags_, readTimeout_, errBuf);
	if (pcap_ == nullptr) {
		SET_ERR(GErr::RETURN_NULL, errBuf);
		return false;
	}

	return GPcap::doOpen();
}

bool GPcapDevice::doClose() {
	if (!enabled_) return true;

	intf_ = nullptr;

	return GPcap::doClose();
}

GPacket::Result GPcapDevice::read(GPacket* packet) {
	GPacket::Result res = GPcap::read(packet);
	if (res == GPacket::TimeOut) QThread::msleep(waitTimeout_);
	return res;
}

#ifdef QT_GUI_LIB

#include "base/prop/gpropitem-interface.h"
GPropItem* GPcapDevice::propCreateItem(GPropItemParam* param) {
	if (QString(param->mpro_.name()) == "intfName") {
		GPropItemInterface* res = new GPropItemInterface(param);
		return res;
	}
	return GObj::propCreateItem(param);
}

#endif // QT_GUI_LIB
