// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include <QThread>
#include "gbase.h"

// ----------------------------------------------------------------------------
// GThread
// ----------------------------------------------------------------------------
struct GThread : QThread {
  GThread(QObject *parent = nullptr) : QThread(parent) {}
  bool wait(unsigned long time = G_TIMEOUT);
};
