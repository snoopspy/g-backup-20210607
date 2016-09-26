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
  struct Handler {
    struct sigaction myAction_;
    struct sigaction oldAction_;
  };
  typedef QHash<int, Handler*> Handlers;

  Handlers handlers_;

  static void _sigFunc(int signo);

public:
  bool setup(int signo);
  static GSignal* instance();

signals:
  void signaled(int signo);
};
