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

#include "base/gstateobj.h"

// ----------------------------------------------------------------------------
// GCommand
// ----------------------------------------------------------------------------
struct G_EXPORT GCommand : GStateObj {
  Q_OBJECT
  Q_PROPERTY(QStringList openCommandList MEMBER openCommandList_)
  Q_PROPERTY(QStringList closeCommandList MEMBER closeCommandList_)

public:
  Q_INVOKABLE GCommand(QObject* parent = nullptr);
  ~GCommand() override;

protected:
  bool doOpen() override;
  bool doClose() override;

protected:
  QStringList openCommandList_;
  QStringList closeCommandList_;
};
