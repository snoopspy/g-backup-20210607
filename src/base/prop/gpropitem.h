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

#include <QDebug>
#include <QMetaProperty>
#include <QJsonObject>
#include <QTreeWidgetItem>

// ----------------------------------------------------------------------------
// GPropItem
// ----------------------------------------------------------------------------
struct GPropItem : public QObject {
  virtual void init(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, QObject* object, QMetaProperty mpro);
  virtual void update() = 0;

  QTreeWidgetItem* item_{nullptr};
  QObject* object_{nullptr};
  QMetaProperty mpro_;
};

#endif // QT_GUI_LIB
