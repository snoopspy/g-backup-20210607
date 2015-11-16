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

#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include "gpropitemwidget.h"

// ----------------------------------------------------------------------------
// GPropItemFilePath
// ----------------------------------------------------------------------------
struct GPropItemFilePath : public GPropItemWidget {
  Q_OBJECT

public:
  GPropItemFilePath(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro);
  void update() override;

public:
  QHBoxLayout* layout_;
  QLineEdit* lineEdit_;
  QToolButton* toolButton_;

protected slots:
  void myEditingFinished();
  void myToolButtonClicked(bool checked);
};

#endif // QT_GUI_LIB
