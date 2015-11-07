#include <QDebug>
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

bool GRtm::loadFromSystem() {
  clear();

  char buf[BUFSIZ];
  const char* command = "route -n";

  FILE* fp = popen(command, "r");
  if (fp == nullptr) {
    qWarning() << QString("popen(%s) return nullptr").arg(command);
    return false;
  }

  while (true)
  {
    char* p = fgets(buf, BUFSIZ, fp);
    if (p == nullptr) break;
    QString s = buf;

    GRtmEntry entry;
    static QRegExp regex(
      /* (1) Destination */ "([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)[ ]+"\
      /* (2) Gateway     */ "([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)[ ]+"\
      /* (3) Genmask     */ "([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+)[ ]+"\
      /* (X) Flags       */ "[A-Za-z]+[ ]+"\
      /* (4) Metric      */ "([0-9]+)[ ]+"\
      /* (X) Ref         */ "[0-9]+[ ]+"\
      /* (X) Use         */ "[0-9]+[ ]+"\
      /* (5) Iface       */ "([A-Za-z0-9]+)");

    if (regex.indexIn(s) == -1) continue;

    entry.dst_      = regex.cap(1);
    entry.mask_     = regex.cap(3);
    entry.gateway_  = regex.cap(2);
    entry.intf_     = regex.cap(5);
    entry.metric_   = regex.cap(4).toInt();

    if (entry.gateway_ == 0xFFFFFFFF) entry.gateway_ = (quint32)0;
    push_back(entry);
  }
  pclose(fp);

  return true;
}

GRtmEntry* GRtm::getBestEntry(GIp ip) {
  GRtmEntry* res = nullptr;

  int _count = count();
  for (int i = 0; i < _count; i++)
  {
    GRtmEntry& entry = (GRtmEntry&)at(i);

    if ((entry.dst_ & entry.mask_) != (ip & entry.mask_)) continue;
    if (res == nullptr)
    {
      res = &entry;
      continue;
    }
    if (entry.mask_ > res->mask_)
    {
      res = &entry;
      continue;
    } else
    if (entry.mask_ == res->mask_)
    {
      if (entry.metric_ < res->metric_)
      {
        res = &entry;
        continue;
      }
    }
  }

  return res;
}

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
      QString(entry.metric_)
    );
  }
}

TEST(GRtm, bestTest) {
  GRtm& rtm = GRtm::instance();
  GRtmEntry* entry = rtm.getBestEntry("8.8.8.8");
  EXPECT_NE(entry,nullptr);
  qDebug() << entry->intf_;
}

#endif // GTEST
