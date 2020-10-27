#include <QDebug>
#include <QProcess>
#include <QTextStream>
#include "grtm.h"

// ----------------------------------------------------------------------------
// GRtmEntry
// ----------------------------------------------------------------------------
GNetIntf* GRtmEntry::intf() const {
	GRtm::instance().init();
	return intf_;
}

bool GRtmEntry::operator==(const GRtmEntry& r) const {
	if (dst_ != r.dst_) return false;
	if (mask_ != r.mask_) return false;
	if (gateway_ != r.gateway_) return false;
	if (metric_ != r.metric_) return false;
	if (intf_ != r.intf_) return false;
	return true;
};

uint qHash(GRtmEntry q) {
	return uint(q.dst() + q.mask() + q.gateway() + uint32_t(q.metric()));
}

// ----------------------------------------------------------------------------
// GRtm
// ----------------------------------------------------------------------------
GRtm::GRtm() {
}

GRtm::~GRtm() {
	clear();
}

GIp GRtm::findGateway(QString intfName, GIp ip) {
	for (GRtmEntry& entry: *this) {
		if (entry.intf()->name() != intfName) continue;
		if (entry.gateway_ == 0) continue;
		if (entry.gateway_ == ip) continue;
		return entry.gateway_;
	}
	return GIp(0);
}

GRtmEntry* GRtm::getBestEntry(GIp ip) {
	GRtmEntry* res = nullptr;

	int _count = count();
	for (int i = 0; i < _count; i++) {
		GRtmEntry& entry = const_cast<GRtmEntry&>(at(i));

		if ((entry.dst_ & entry.mask_) != (ip & entry.mask_)) continue;
		if (res == nullptr) {
			res = &entry;
			continue;
		}
		if (entry.mask_ > res->mask_) {
			res = &entry;
			continue;
		} else
		if (entry.mask_ == res->mask_) {
			if (entry.metric_ < res->metric_) {
				res = &entry;
				continue;
			}
		}
	}

	return res;
}

#ifdef Q_OS_LINUX
#include "net/_linux/grtmlinux.h"
GRtm& GRtm::instance() {
	static GRtmLinux rtm;
	return rtm;
}
#endif
#ifdef Q_OS_WIN
#include "net/_win/grtmwin32.h"
GRtm& GRtm::instance() {
	static GRtmWin32 rtm;
	return rtm;
}
#endif

// ----------------------------------------------------------------------------
// GTEST
// ----------------------------------------------------------------------------
#ifdef GTEST
#include <gtest/gtest.h>

TEST(GRtm, loadTest) {
	GRtm& rtm = GRtm::instance();
	qDebug() << "Routing Table Manager : count =" << rtm.count();
	for (GRtm::iterator it = rtm.begin(); it != rtm.end(); it++) {
		GRtmEntry& entry = *it;
		qDebug() << QString("dst=%1 mask=%2 gateway=%3 intf=%4 metric=%5").arg(
			QString(entry.dst()),
			QString(entry.mask()),
			QString(entry.gateway()),
			entry.intf()->name(),
			QString::number(entry.metric())
			);
	}
}

TEST(GRtm, bestTest) {
	GRtm& rtm = GRtm::instance();
	GRtmEntry* entry = rtm.getBestEntry(QString("8.8.8.8"));
	EXPECT_NE(entry, nullptr);
	qDebug() << "best entry for 8.8.8.8 is" << entry->intf()->name();
}

#endif // GTEST
