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
  typedef _crt_signal_t Handler;
  typedef QHash<int, Handler> Handlers;
  Handlers handlers_;
  static void _signalHandler(int signo);

public:
  void setup(int signo);
  static GSignal& instance();

signals:
  void signaled(int signo);
};
