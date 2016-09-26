#include "gnetfilter.h"
#include <libnetfilter_queue/libnetfilter_queue.h>

// ----------------------------------------------------------------------------
// GNetFilter
// ----------------------------------------------------------------------------
GNetFilter::GNetFilter(QObject* parent) : GCapture(parent) {
  qDebug() << "GNetFilter::GNetFilter"; // gilgil temp 2016.09.25
}

GNetFilter::~GNetFilter()  {
  qDebug() << "GNetFilter::~GNetFilter"; // gilgil temp 2016.09.25
  close();
}

bool GNetFilter::doOpen() {
  qDebug() << "GNetFilter::~doOpen"; // gilgil temp 2016.09.25
  return true;
}

bool GNetFilter::doClose() {
  qDebug() << "GNetFilter::~doClose"; // gilgil temp 2016.09.25
  return true;
}
