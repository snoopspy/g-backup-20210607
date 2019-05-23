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

#include <QJsonObject>
#include "base/gbase.h"

// ----------------------------------------------------------------------------
// GJson
// ----------------------------------------------------------------------------
namespace G_EXPORT GJson {
  QJsonObject loadFromFile(QString fileName = "");
  bool saveToFile(QJsonObject jo, QString fileName = "");
  QString defaultFileName();
}

#include <QList>
// ----------------------------------------------------------------------------
// QList<int>
// ----------------------------------------------------------------------------
QJsonValueRef G_EXPORT operator << (QJsonValueRef ref, const QList<int>& intList);
const QJsonValue G_EXPORT operator >> (const QJsonValue val, QList<int>& intList);

// ----------------------------------------------------------------------------
// GProp
// ----------------------------------------------------------------------------
struct GProp;
QJsonValueRef G_EXPORT operator << (QJsonValueRef ref, const GProp& prop);
const QJsonValue G_EXPORT operator >> (const QJsonValue val, GProp& prop);

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GWidgetRect
// ----------------------------------------------------------------------------
#include <QWidget>
namespace G_EXPORT GJson {
  struct GWidgetRect {
    GWidgetRect(QWidget* widget) { widget_ = widget; }
    QWidget* widget_;
  };
  GWidgetRect rect(QWidget* widget);
}
QJsonValueRef G_EXPORT operator << (QJsonValueRef ref, const GJson::GWidgetRect&& rect);
const QJsonValue G_EXPORT operator >> (const QJsonValue val, GJson::GWidgetRect&& rect);

// ----------------------------------------------------------------------------
// GSplitterSizes
// ----------------------------------------------------------------------------
#include <QSplitter>
namespace G_EXPORT GJson {
  struct GSplitterSizes {
    GSplitterSizes(QSplitter* splitter) { splitter_ = splitter; }
    QSplitter* splitter_;
  };
  GSplitterSizes splitterSizes(QSplitter* splitter);
}
QJsonValueRef G_EXPORT operator << (QJsonValueRef ref, const GJson::GSplitterSizes&& sizes);
const QJsonValue G_EXPORT operator >> (const QJsonValue val, GJson::GSplitterSizes&& sizes);

// ----------------------------------------------------------------------------
// GTreeViewHeaderSizes
// ----------------------------------------------------------------------------
#include <QTreeView>
#include <QHeaderView>
namespace G_EXPORT GJson {
  struct GTreeViewHeaderSizes {
    GTreeViewHeaderSizes(QTreeView* treeView) { treeView_ = treeView; }
    QTreeView* treeView_;
  };
  GTreeViewHeaderSizes headerSizes(QTreeView* treeView);
}
QJsonValueRef G_EXPORT operator << (QJsonValueRef ref, const GJson::GTreeViewHeaderSizes&& headerSizes);
const QJsonValue G_EXPORT operator >> (const QJsonValue val, GJson::GTreeViewHeaderSizes&& headerSizes);

#endif // QT_GUI_LIB
