#include "grtmlinux.h"
#include "net/gnetintf.h"

// ----------------------------------------------------------------------------
// GRtmLinux
// ----------------------------------------------------------------------------
GRtmLinux::GRtmLinux() : GRtm() {
	QString command = "cat /proc/net/route";
	QProcess p;
	p.start(command);
	if (!p.waitForFinished()) {
		qWarning() << QString("waitForFinished(%1) return false").arg(command);
		return;
	}

	QList<QByteArray> baList = p.readAll().split('\n');
	bool firstLine = true;
	QList<QString> fields;

	for (QByteArray ba: baList) {
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
					intfNames_.push_back(value);
				}
				else if (field == "Destination")
					entry.dst_ = ntohl(value.toUInt(nullptr, 16));
				else if (field == "Mask")
					entry.mask_ = ntohl(value.toUInt(nullptr, 16));
				else if (field == "Gateway")
					entry.gateway_ = ntohl(value.toUInt(nullptr, 16));
				else if (field == "Metric")
					entry.metric_ = value.toInt(nullptr, 16);
			}
			if (appending)
				append(entry);
		}
	}
}

GRtmLinux::~GRtmLinux() {
	clear();
}

void GRtmLinux::init() {
	if (initialized_) return;
	initialized_ = true;

	Q_ASSERT(count() == intfNames_.count());
	for (int i = 0; i < count(); i++) {
		GRtmEntry& entry = const_cast<GRtmEntry&>(at(i));
		QString intfName = intfNames_.at(i);
		GNetIntf* netIntf = GNetIntfs::instance().findByName(intfName);
		if (netIntf == nullptr) {
			QString msg = QString("GNetIntfs::instance().findByName(%1) return false").arg(intfName);
			qFatal("%s", qPrintable(msg));
		}
		entry.intf_ = netIntf;
	}
}
