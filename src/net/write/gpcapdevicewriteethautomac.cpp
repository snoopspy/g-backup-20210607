#include "gpcapdevicewriteethautomac.h"
#include <QMap>
#include "net/gatm.h"

// ----------------------------------------------------------------------------
// GSyncPcapDeviceMgr
// ----------------------------------------------------------------------------
typedef QMap<QString /*devName*/, GSyncPcapDevice*> GSyncPcapDeviceMap;
struct GSyncPcapDeviceMgr : GSyncPcapDeviceMap {
private: // singleton
	GSyncPcapDeviceMgr() {}
	virtual ~GSyncPcapDeviceMgr() {
		GSyncPcapDeviceMap::iterator it = begin();
		while (it != end()) {
			delete *it;
			it = erase(it);
		}
	}
public:
	static GSyncPcapDeviceMgr& instance() {
		static GSyncPcapDeviceMgr syncPcapDeviceMgr;
		return syncPcapDeviceMgr;
	}
};

// ----------------------------------------------------------------------------
// GPcapDeviceWriteEthAutoMac
// ----------------------------------------------------------------------------
bool GPcapDeviceWriteEthAutoMac::doOpen() {
	if (!GPcapDeviceWrite::doOpen())
		return false;

	if (dataLinkType_ != GPacket::Eth) {
		QString msg = QString("data link layer(%1) must be GPacket::Eth").arg(GPacket::dataLinkTypeToString(dataLinkType_));
		SET_ERR(GErr::FAIL, msg);
		return false;
	}

	Q_ASSERT(intf_ != nullptr);
	myMac_ = intf_->mac();
	if (myMac_.isNull()) {
		QString msg = QString("not specified mac");
		SET_ERR(GErr::VALUE_IS_NOT_ZERO, msg);
		return false;
	}

	GSyncPcapDeviceMgr& mgr = GSyncPcapDeviceMgr::instance();
	GSyncPcapDeviceMap::iterator it = mgr.find(devName_);
	if (it == mgr.end()) {
		GSyncPcapDevice* device = new GSyncPcapDevice;
		device->devName_ = devName_;
		if (!device->active() && !device->open()) {
			err = device->err;
			delete device;
			return false;
		}
		it = mgr.insert(devName_, device);
	}
	device_ = it.value();
	Q_ASSERT(device_ != nullptr);

	return true;
}

bool GPcapDeviceWriteEthAutoMac::doClose() {
	return GPcapDeviceWrite::doClose();
}

GPacket::Result GPcapDeviceWriteEthAutoMac::writeEth(GPacket* packet) {
	GBuf oldBuf = packet->buf_;
	GBuf newBuf;
	GEthHdr* ethHdr = nullptr; // for remove warning
	switch (packet->dataLinkType()) {
		case GPacket::Eth:
			newBuf = packet->buf_;
			ethHdr = PEthHdr(oldBuf.data_);
			break;
		case GPacket::Ip:
			newBuf.data_ = temp_;
			newBuf.size_ = sizeof(GEthHdr) + oldBuf.size_;
			Q_ASSERT(newBuf.size_ <= MAXBUF);
			ethHdr = PEthHdr(temp_);
			memcpy(temp_ + sizeof(GEthHdr), oldBuf.data_, oldBuf.size_);
			break;
		case GPacket::Dot11:
		case GPacket::Null: {
			QString msg = QString("not supported data link type(%1)").arg(GPacket::dataLinkTypeToString(dataLinkType_));
			SET_ERR(GErr::NOT_SUPPORTED, msg);
			return GPacket::Fail;
		}
	}
	ethHdr->smac_ = myMac_;
	GIpHdr* ipHdr = packet->ipHdr_;
	if (ipHdr == nullptr) {
		SET_ERR(GErr::VALUE_IS_NULL, "ip header is null");
		return GPacket::Fail;
	}
	GIp dip = ipHdr->dip();
	GIp adjIp = intf_->getAdjIp(dip);
	GAtm& atm = GAtm::instance();
	GAtmMap::iterator it = atm.find(adjIp);
	if (it == atm.end()) {
		GAtm atmOne;
		GMac mac = atmOne.waitOne(adjIp, device_);
		if (mac.isNull()) {
			qCritical() << QString("can not resolve mac for ip %1").arg(QString(adjIp));
			return GPacket::Fail;
		}
		it = atm.insert(adjIp, mac);
		if (!active()) {
			QString msg = QString("not opened state %1").arg(int(state_));
			SET_ERR(GErr::NOT_OPENED_STATE, msg); // gilgil temp 2019.06.02
			return GPacket::Fail;
		}
	}
	GMac dmac = it.value();
	ethHdr->dmac_ = dmac;
	ethHdr->type_ = htons(type_);
	packet->buf_ = newBuf;
	GPacket::Result res = GPcapDeviceWrite::write(packet);
	packet->buf_ = oldBuf;
	return res;
}
