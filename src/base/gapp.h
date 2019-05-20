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
  #include <GPluginFactory>
#else
  #include <QCoreApplication>
#endif // QT_GUI_LIB

// ----------------------------------------------------------------------------
// GApp
// ----------------------------------------------------------------------------
#ifdef QT_GUI_LIB
struct GApp : QApplication {
#else
struct GApp : QCoreApplication {
#endif // QT_GUI_LIB

public:
  GApp(int &argc, char **argv);
  static void init();

#ifdef QT_GUI_LIB
  static bool exec_(GObj* obj);
  static bool exec_(GGraph* graph = nullptr, GPluginFactory* pluginFactory = nullptr);
#endif // QT_GUI_LIB
};
