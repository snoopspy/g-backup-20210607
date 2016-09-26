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

#include <csignal>
#include <QCoreApplication>

// ----------------------------------------------------------------------------
// GApp
// ----------------------------------------------------------------------------
struct GApp : QCoreApplication {
  Q_OBJECT

public:
  GApp(int argc, char* argv[]);
  ~GApp() override;
  void init();
  static GApp* instance();

protected:
  void signal(int signo);

signals:
  void signaled(int signo);
};
