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

#include <QJsonArray>
#include <QJsonObject>
#include <QMetaObject>
#include <QMetaProperty>
#include "gpropitem.h"

// ----------------------------------------------------------------------------
// GProp
// ----------------------------------------------------------------------------
struct GProp {
  virtual void load(QJsonObject jo);
  virtual void save(QJsonObject& jo);

  virtual bool load(QJsonObject jo, QMetaProperty mpro);
  virtual bool save(QJsonObject& jo, QMetaProperty mpro);

#ifdef QT_GUI_LIB
  virtual GPropItem* createPropItem(GPropItemParam param);
  virtual void createPropItems(QTreeWidget* treeWidget, QTreeWidgetItem* parent, QObject* object);
#endif // QT_GUI_LIB

};
