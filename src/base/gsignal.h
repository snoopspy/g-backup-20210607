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
#include <QHash>
#include <QObject>

// ----------------------------------------------------------------------------
// GSignal
// ----------------------------------------------------------------------------
struct GSignal : QObject {
  Q_OBJECT

private: // singleton
  GSignal();
  ~GSignal() override;

protected:
#ifdef Q_OS_LINUX
  typedef __sighandler_t Handler;
#endif
#ifdef Q_OS_WIN
  typedef _crt_signal_t Handler;
#endif
  typedef QHash<int, Handler> Handlers;
  Handlers handlers_;
  static void _signalHandler(int signo);

public:
  void setup(int signo);
  static GSignal& instance();

signals:
  void signaled(int signo);
};
