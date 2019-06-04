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

#ifdef QT_GUI_LIB
  #include <QApplication>
#else
  #include <QCoreApplication>
#endif // QT_GUI_LIB

#include "base/graph/ggraph.h"
#include "base/graph/gpluginfactory.h"

// ----------------------------------------------------------------------------
// GApp
// ----------------------------------------------------------------------------
#ifdef QT_GUI_LIB
struct G_EXPORT GApp : QApplication {
#else
struct G_EXPORT GApp : QCoreApplication {
#endif // QT_GUI_LIB

public:
  GApp(int &argc, char** argv);
  static void init();

#ifdef QT_GUI_LIB
  static bool execObj(GObj* obj);
  static bool execFactory(GPluginFactory* pluginFactory = nullptr);
  static bool execGraphFactory(GGraph* graph, GPluginFactory* pluginFactory = nullptr);
#endif // QT_GUI_LIB
};
