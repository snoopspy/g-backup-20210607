#include "grtmlinux.h"
#include "net/gnetintf.h"

// ----------------------------------------------------------------------------
// GRtmLinux
// ----------------------------------------------------------------------------
bool GRtmLinux::init() {
  if (initialized_) return false;
  initialized_ = true;
  clear();

  QString command = "cat /proc/net/route";
  QProcess p;
  p.start(command);
  if (!p.waitForFinished()) {
    qWarning() << QString("waitForFinished(%1) return false").arg(command);
    return false;
  }

  QList<QByteArray> baList = p.readAll().split('\n');
  bool firstLine = true;
  QList<QString> fields;

  foreach (QByteArray ba, baList) {
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
      for (int i = 0; i < fields.count(); i++) {
        QString field = fields.at(i);
        QString value;
        ts >> value;
        if (value == "") break;
        if (field == "Iface") {
          QString intfName = value;
          GNetIntf* netIntf = GNetIntfs::instance().findByName(intfName);
          if (netIntf == nullptr) {
            qCritical() << QString("GNetIntfs::instance().findByName(%1) return false").arg(intfName);
            continue;
          }
          entry.intf_ = netIntf;
        } else if (field == "Destination")
          entry.dst_ = ntohl(value.toUInt(nullptr, 16));
        else if (field == "Gateway")
          entry.gateway_ = ntohl(value.toUInt(nullptr, 16));
        else if (field == "Mask")
          entry.mask_ = ntohl(value.toUInt(nullptr, 16));
        else if (field == "Metric")
          entry.metric_ = value.toInt(nullptr, 16);
      }
      if (entry.intf_ != nullptr) // if not empty
        append(entry);
    }
  }
  return true;
}
