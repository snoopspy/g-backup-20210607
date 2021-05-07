#include "gremotepcapdevice.h"

// ----------------------------------------------------------------------------
// GRemotePcapDevice
// ----------------------------------------------------------------------------
GRemotePcapDevice::GRemotePcapDevice(QObject* parent) : GRemotePcapCapture(parent) {
	GRtmEntry* entry = GRemoteNetInfo::instance(ip_, port_).rtm().getBestEntry(QString("8.8.8.8"));
	if (entry != nullptr)
		intfName_ = entry->intf()->name();
}

GRemotePcapDevice::~GRemotePcapDevice() {
	close();
}

bool GRemotePcapDevice::doOpen() {
	if (!enabled_) return true;

	if (intfName_ == "") {
		SET_ERR(GErr::INTERFACE_NAME_NOT_SPECIFIED, "intfName is not specified");
		return false;
	}

	demonClient_ = new GDemonClient(std::string(qPrintable(ip_)), port_);
	GDemon::PcapOpenRep rep = demonClient_->pcapOpen(std::string(qPrintable(filter_)), std::string(qPrintable(intfName_)), snapLen_, flags_, readTimeout_, waitTimeout_, true);
	if (!rep.result_) {
		SET_ERR(GErr::FAIL, demonClient_->error_.data());
		delete demonClient_; demonClient_ = nullptr;
		return false;
	}

	intf_ = GRemoteNetInfo::instance(ip_, port_).interfaceList().findByName(intfName_);
	if (intf_ == nullptr) {
		QString msg = QString("can not find interface for %1").arg(intfName_);
		SET_ERR(GErr::VALUE_IS_NULL, msg);
		return false;
	}

	dlt_ = GPacket::intToDlt(rep.dataLink_);

	return GRemotePcapCapture::doOpen();
}

bool GRemotePcapDevice::doClose() {
	if (!enabled_) return true;

	intf_ = nullptr;

	return GRemotePcapCapture::doClose();
}

#ifdef QT_GUI_LIB

#include "base/prop/gpropitem-interface.h"
GPropItem* GRemotePcapDevice::propCreateItem(GPropItemParam* param) {
	if (QString(param->mpro_.name()) == "intfName") {
		GPropItemInterface* res = new GPropItemInterface(param);
		res->comboBox_->setEditable(true);
		return res;
	}
	return GObj::propCreateItem(param);
}

#endif // QT_GUI_LIB
