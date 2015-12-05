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

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include "gpropitem_widget.h"

// ----------------------------------------------------------------------------
// GPropItemFilePath
// ----------------------------------------------------------------------------
struct GPropItemFilePath : public GPropItemWidget {
  Q_OBJECT

public:
  GPropItemFilePath(GPropItemParam param);
  void update() override;

public:
  QHBoxLayout* layout_{nullptr};
  QLineEdit* lineEdit_{nullptr};
  QToolButton* toolButton_{nullptr};
  QFileDialog* fd_{nullptr};

protected slots:
  void myEditingFinished();
  void myToolButtonClicked(bool checked);
};

#endif // QT_GUI_LIB
