#include <QDebug>
#include <QProcess>
#include <QTextStream>
#include "grtm.h"

// ----------------------------------------------------------------------------
// GRtm
// ----------------------------------------------------------------------------
GRtm::GRtm() {
  loadFromSystem();
}

GRtm::~GRtm() {
  clear();
}

#ifdef Q_OS_LINUX
bool GRtm::loadFromSystem() {
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
        if (field == "Iface")
          entry.intf_ = value;
        else if (field == "Destination")
          entry.dst_ = ntohl(value.toUInt(nullptr, 16));
        else if (field == "Gateway")
          entry.gateway_ = ntohl(value.toUInt(nullptr, 16));
        else if (field == "Mask")
          entry.mask_ = ntohl(value.toUInt(nullptr, 16));
        else if (field == "Metric")
          entry.metric_ = value.toInt(nullptr, 16);
      }
      if (entry.intf_ != "") // if not empty
        append(entry);
    }
  }
  return true;
}
#endif
#ifdef Q_OS_WIN
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

bool GRtm::loadFromSystem() {
  PMIB_IPFORWARDTABLE pIpForwardTable;

  pIpForwardTable = PMIB_IPFORWARDTABLE(malloc(sizeof (MIB_IPFORWARDTABLE)));
  if (pIpForwardTable == nullptr) {
    qCritical() << "Error allocating memory";
    return false;
  }

  DWORD dwSize = 0;
  if (GetIpForwardTable(pIpForwardTable, &dwSize, 0) ==
      ERROR_INSUFFICIENT_BUFFER) {
    free(pIpForwardTable);
    pIpForwardTable = PMIB_IPFORWARDTABLE(malloc(dwSize));
    if (pIpForwardTable == nullptr) {
      qCritical() << "Error allocating memory";
      return false;
    }
  }

  DWORD dwRetVal = GetIpForwardTable(pIpForwardTable, &dwSize, 0);
  if (dwRetVal != NO_ERROR) {
    qCritical() << "GetIpForwardTable failed";
    free(pIpForwardTable);
    return false;
  }

  for (DWORD i = 0; i < pIpForwardTable->dwNumEntries; i++) {
    PMIB_IPFORWARDROW table = &pIpForwardTable->table[i];
    GRtmEntry entry;
    /* Convert IPv4 addresses to strings */
    entry.dst_ = ntohl(table->dwForwardDest);
    entry.mask_ = ntohl(table->dwForwardMask);
    entry.gateway_ = ntohl(table->dwForwardNextHop);
    entry.metric_ = int(table->dwForwardMetric1);
    append(entry);
  }
  free(pIpForwardTable);
  return true;
}
#endif

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
GIp GRtm::getGateway(QString intf) {
  for (GRtm::iterator it = begin(); it != end(); it++) {
    GRtmEntry& entry = *it;
    if (entry.intf_ == intf && entry.gateway_ != 0)
      return entry.gateway_;
  }
  return GIp(uint32_t(0));
}
#endif
#ifdef Q_OS_WIN
GIp GRtm::getGateway(GIp intf) {
  for (GRtm::iterator it = begin(); it != end(); it++) {
    GRtmEntry& entry = *it;
    if (entry.intf_ == intf && entry.gateway_ != 0)
      return entry.gateway_;
  }
  return GIp(uint32_t(0));
}
#endif

GRtm& GRtm::instance() {
  static GRtm rtm;
  return rtm;
}

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
      QString(entry.dst_),
      QString(entry.mask_),
      QString(entry.gateway_),
      QString(entry.intf_),
      QString::number(entry.metric_)
    );
  }
}

TEST(GRtm, bestTest) {
  GRtm& rtm = GRtm::instance();
  GRtmEntry* entry = rtm.getBestEntry("8.8.8.8");
  EXPECT_NE(entry, nullptr);
  qDebug() << "best entry for 8.8.8.8 is" << entry->intf_;
}

#endif // GTEST
