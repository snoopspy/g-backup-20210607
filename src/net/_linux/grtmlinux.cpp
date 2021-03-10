#include "grtmlinux.h"
#include "net/ginterface.h"

// ----------------------------------------------------------------------------
// GRtmLinux
// ----------------------------------------------------------------------------
GRtmLinux::GRtmLinux() : GRtm() {
	QString command = "cat";
	QStringList args = QStringList{"/proc/net/route"};
	QProcess p;
	p.start(command, args);
	if (!p.waitForFinished()) {
		qWarning() << QString("waitForFinished(%1) return false").arg(command) << args;
		return;
	}
	QList<QByteArray> baList = p.readAll().split('\n');
	bool firstLine = true;
	QList<QString> fields;

	for (QByteArray& ba: baList) {
		QTextStream ts(ba);
		if (firstLine) {
			firstLine = false;
			while (true) {
				QString field;
				ts >> field;
				if (field == "") break;
				fields.append(field);
			}
		} else {
			GRtmEntry entry;
			bool appending = false;
			for (int i = 0; i < fields.count(); i++) {
				QString field = fields.at(i);
				QString value;
				ts >> value;
				if (value == "") break;
				if (field == "Iface") {
					appending = true;
					Q_ASSERT(value != "");
					entry.intfName_ = value;
				}
				else if (field == "Destination")
					entry.dst_ = ntohl(value.toUInt(nullptr, 16));
				else if (field == "Mask")
					entry.mask_ = ntohl(value.toUInt(nullptr, 16));
				else if (field == "Gateway")
					entry.gateway_ = ntohl(value.toUInt(nullptr, 16));
				else if (field == "Metric")
					entry.metric_ = value.toInt(nullptr, 16);
				// intf_ is initialized in GNetInfo
			}
			if (appending)
				append(entry);
		}
	}


	#ifdef GILGIL_ANDROID_DEBUG
	// for gathering gateway
	command = "ip";
	args = QStringList{"route", "show", "table", "0"};
	p.start(command, args);
	if (!p.waitForFinished()) {
		qWarning() << QString("waitForFinished(%1) return false").arg(command) << args;
		return;
	}
	baList = p.readAll().split('\n');
	for (QByteArray& ba: baList) {
		char gateway[256];
		char dev[256];
		// default via 10.2.2.1 dev wlan0  table 1021  proto static
		int res = sscanf(ba.data(), "default via %s dev %s", gateway, dev);
		if (res == 2) {
			GRtmEntry entry;
			entry.intfName_ = dev;
			entry.dst_ = 0;
			entry.mask_ = 0;
			entry.gateway_ = GIp(gateway);
			entry.metric_ = 0;
			append(entry);
		}
	}
	#endif // GILGIL_ANDROID_DEBUG
}

GRtmLinux::~GRtmLinux() {
	clear();
}

