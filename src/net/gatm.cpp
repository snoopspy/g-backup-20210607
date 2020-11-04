#include "gatm.h"
#include <QElapsedTimer>

// ----------------------------------------------------------------------------
// GAtm
// ----------------------------------------------------------------------------
bool GAtm::allResolved() {
	for (GMac& mac: *this)
		if (mac.isNull()) return false;
	return true;
}

void GAtm::deleteUnresolved() {
	for (GAtmMap::iterator it = begin(); it != end();) {
		GMac mac = it.value();
		if (mac.isNull()) {
			it = erase(it);
		} else {
			it++;
		}
	}
}

bool GAtm::waitAll(GPcapDevice* pcapDevice, GDuration timeout) {
	if (allResolved()) return true;

	if (!pcapDevice->active()) {
		qWarning() << QString("not opened state %1").arg(pcapDevice->metaObject()->className());
		return false;
	}

	GPacket::DataLinkType dataLinkType = pcapDevice->dataLinkType();
	if (dataLinkType != GPacket::Eth) {
		qWarning() << QString("invalid dataLinkType %1").arg(GPacket::dataLinkTypeToString(dataLinkType));
		return false;
	}

	GNetIntf* intf = GNetIntf::all().findByName(pcapDevice->devName_);
	if (intf == nullptr) {
		qWarning() << QString("can not find intf for %1").arg(pcapDevice->devName_);
		return false;
	}

	SendThread thread(this, pcapDevice, intf, timeout);
	thread.start();

	bool succeed = false;
	while (true) {
		if (allResolved()) {
			succeed = true;
			break;
		}

		if (thread.isFinished()) {
			QString msg = "can not resolve all ip";
			for (GAtmMap::iterator it = begin(); it != end(); it++) {
				GMac mac = it.value();
				if (mac.isNull()) {
					GIp ip = it.key();
					msg += " ";
					msg += QString(ip);
				}
			}
			qWarning() << msg;
			break;
		}

		GEthPacket packet;
		GPacket::Result res = pcapDevice->read(&packet);
		if (res == GPacket::Eof) {
			qWarning() << "pcapDevice->read return GPacket::Eof";
			break;
		} else
		if (res == GPacket::Fail) {
			qWarning() << "pcapDevice->read return GPacket::Eof";
			break;
		} else
		if (res == GPacket::TimeOut) {
			continue;
		}

		GArpHdr* arpHdr = packet.arpHdr_;
		if (arpHdr == nullptr) continue;
		if (arpHdr->op() != GArpHdr::Reply) continue;

		GIp sip = arpHdr->sip();
		GMac smac = arpHdr->smac();
		GAtmMap::iterator it = find(sip);
		if (it != end()) {
			it.value() = smac;
			qDebug() << QString("resolved ip:%1 mac:%2").arg(QString(it.key()), QString(it.value()));
			if (allResolved()) {
				succeed = true;
				break;
			}
		}
	}
	thread.we_.wakeAll();
	thread.wait();
	return succeed;
}

bool GAtm::sendQueries(GPcapDevice* pcapDevice, GNetIntf* intf) {
	GEthArpHdr query;
	query.ethHdr_.dmac_ = GMac::broadcastMac();
	query.ethHdr_.smac_ = intf->mac();
	query.ethHdr_.type_ = htons(GEthHdr::Arp);

	query.arpHdr_.hrd_ = htons(GArpHdr::ETHER);
	query.arpHdr_.pro_ = htons(GEthHdr::Ip4);
	query.arpHdr_.hln_ = sizeof(GMac);
	query.arpHdr_.pln_ = sizeof(GIp);
	query.arpHdr_.op_ = htons(GArpHdr::Request);
	query.arpHdr_.smac_ = intf->mac();
	query.arpHdr_.sip_ = htonl(intf->ip());
	query.arpHdr_.tmac_ = GMac::nullMac();
	GBuf queryBuf(pbyte(&query), sizeof(query));

	for (GAtmMap::iterator it = begin(); it != end(); it++) {
		GIp ip = it.key();
		GMac mac = it.value();
		if (mac.isNull()) {
			query.arpHdr_.tip_ = htonl(ip);
			GPacket::Result res = pcapDevice->write(queryBuf);
			if (res != GPacket::Ok) {
				qCritical() << "pcapDevice->write return GPacket::Fail";
				return false;
			}
		}
	}
	return true;
}

GAtm& GAtm::instance() {
	static GAtm atm;
	return atm;
}

GMac GAtm::waitOne(GIp ip, GPcapDevice* device, GDuration timeout) {
	GAtmMap::iterator it = find(ip);
	if (it != end()) {
		GMac mac = it.value();
		if (!mac.isNull())
			return mac;
	} else
		it = insert(ip, GMac::nullMac());

	if (!waitAll(device, timeout)) {
		return GMac::nullMac();
	}
	return it.value();
}

// --------------------------------------------------------------------------
// GAtm::SendThread
// --------------------------------------------------------------------------
GAtm::SendThread::SendThread(GAtm* resolve, GPcapDevice* device, GNetIntf* intf, GDuration timeout) {
	atm_ = resolve;
	device_ = device;
	intf_ = intf;
	timeout_ = timeout;
}

void GAtm::SendThread::run() {
	QElapsedTimer timer; timer.start();
	qint64 first = timer.elapsed();
	while (true) {
		if (!atm_->sendQueries(device_, intf_))
			break;
		bool res = we_.wait(1000);
		if (res) break;
		qint64 now = timer.elapsed();
		if (now - first >= qint64(timeout_)) {
			qWarning() << "SendThread::run() timeout elapsed";
			break;
		}
	}
}

// ----------------------------------------------------------------------------
// GTEST
// ----------------------------------------------------------------------------
#ifdef GTEST
#include <gtest/gtest.h>

#include "net/capture/gsyncpcapdevice.h"
TEST(GAtm, resolveTest) {
	GSyncPcapDevice device;

	ASSERT_TRUE(device.open());

	QString devName = device.devName_;
	ASSERT_NE(devName, "");

	GNetIntf* intf = GNetIntf::all().findByName(devName);
	ASSERT_TRUE(intf != nullptr);

	GIp ip = intf->gateway();
	ASSERT_NE(ip, 0);

	GAtm atm;
	GMac mac = atm.waitOne(ip, &device);
	ASSERT_FALSE(mac.isNull());

	qDebug() << QString("ip:%1 mac:%2").arg(QString(ip), QString(mac));
}

#endif // GTEST
