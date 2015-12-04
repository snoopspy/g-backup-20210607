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
#include "gpropitem_widget.h"

// ----------------------------------------------------------------------------
// GPropItemDirPath
// ----------------------------------------------------------------------------
struct GPropItemDirPath : public GPropItemWidget {
  Q_OBJECT

public:
  void init(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, QObject* object, QMetaProperty mpro) override;
  void update() override;

public:
  QHBoxLayout* layout_{nullptr};
  QLineEdit* lineEdit_{nullptr};
  QToolButton* toolButton_{nullptr};

protected slots:
  void myEditingFinished();
  void myToolButtonClicked(bool checked);
};

#endif // QT_GUI_LIB
