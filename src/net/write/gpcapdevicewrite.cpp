#include "gpcapdevicewrite.h"
#include "net/grtm.h"

// ----------------------------------------------------------------------------
// GPcapDeviceWrite
// ----------------------------------------------------------------------------
GPcapDeviceWrite::GPcapDeviceWrite(QObject* parent) : GPcapWrite(parent) {
	GRtmEntry* entry = GRtm::instance().getBestEntry(QString("8.8.8.8"));
	if (entry != nullptr)
		devName_ = entry->intf()->name();
}

GPcapDeviceWrite::~GPcapDeviceWrite() {
	close();
}

bool GPcapDeviceWrite::doOpen() {
	if (devName_ == "") {
		SET_ERR(GErr::DEVICE_NOT_SPECIFIED, "device is not specified");
		return false;
	}

	intf_ = GNetIntf::all().findByName(devName_);
	if (intf_ == nullptr) {
		QString msg = QString("can not find interface for %1").arg(devName_);
		SET_ERR(GErr::VALUE_IS_NULL, msg);
		return false;
	}

	char errBuf[PCAP_ERRBUF_SIZE];
	pcap_ = pcap_open_live(qPrintable(devName_), 0, 0, 0, errBuf);
	if (pcap_ == nullptr) {
		SET_ERR(GErr::RETURN_NULL, errBuf);
		return false;
	}

	int dataLink = pcap_datalink(pcap_);
	dataLinkType_ = GPacket::intToDataLinkType(dataLink);

	return true;
}

bool GPcapDeviceWrite::doClose() {
	if (pcap_ != nullptr) {
		pcap_close(pcap_);
		pcap_ = nullptr;
	}

	intf_ = nullptr;

	return true;
}

GPacket::Result GPcapDeviceWrite::write(GBuf buf) {
	int i = pcap_sendpacket(pcap_, buf.data_, int(buf.size_));
	if (i != 0) {
		SET_ERR(GErr::FAIL, QString("pcap_sendpacket return %1 %2").arg(i).arg(pcap_geterr(pcap_)));
		return GPacket::Fail;
	}
	return GPacket::Ok;
}

GPacket::Result GPcapDeviceWrite::write(GPacket* packet) {
	GPacket::Result res = write(packet->buf_);
	if (res == GPacket::Ok)
		emit written(packet);
	return res;
}

#ifdef QT_GUI_LIB

#include "base/prop/gpropitem-device.h"
GPropItem* GPcapDeviceWrite::propCreateItem(GPropItemParam* param) {
	if (QString(param->mpro_.name()) == "devName") {
		GPropItemDevice* res = new GPropItemDevice(param);
		return res;
	}
	return GObj::propCreateItem(param);
}

#endif // QT_GUI_LIB
