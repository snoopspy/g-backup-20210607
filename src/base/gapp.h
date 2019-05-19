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

protected:
  static void init();
};
