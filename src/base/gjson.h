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

#include <QDebug>
#include <QJsonObject>

// ----------------------------------------------------------------------------
// GJson
// ----------------------------------------------------------------------------
namespace GJson {
  QJsonObject loadFromFile(QString fileName = "");
  bool saveToFile(QJsonObject jo, QString fileName = "");
  QString defaultFileName();
}

#include <QList>
// ----------------------------------------------------------------------------
// QList<int>
// ----------------------------------------------------------------------------
QJsonValueRef operator << (QJsonValueRef ref, const QList<int>& intList);
const QJsonValue operator >> (const QJsonValue val, QList<int>& intList);

// ----------------------------------------------------------------------------
// GObj
// ----------------------------------------------------------------------------
class GObj;
QJsonValueRef operator << (QJsonValueRef ref, const GObj& obj);
const QJsonValue operator >> (const QJsonValue val, GObj& obj);

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GWidgetRect
// ----------------------------------------------------------------------------
#include <QWidget>
namespace GJson {
  struct GWidgetRect {
    GWidgetRect(QWidget* widget) { widget_ = widget; }
    QWidget* widget_;
  };
  GWidgetRect rect(QWidget* widget);
}
QJsonValueRef operator << (QJsonValueRef ref, const GJson::GWidgetRect&& rect);
const QJsonValue operator >> (const QJsonValue val, GJson::GWidgetRect&& rect);

// ----------------------------------------------------------------------------
// GSplitterSizes
// ----------------------------------------------------------------------------
#include <QSplitter>
namespace GJson {
  struct GSplitterSizes {
    GSplitterSizes(QSplitter* splitter) { splitter_ = splitter; }
    QSplitter* splitter_;
  };
  GSplitterSizes headerSizes(QSplitter* splitter);
}
QJsonValueRef operator << (QJsonValueRef ref, const GJson::GSplitterSizes&& sizes);
const QJsonValue operator >> (const QJsonValue val, GJson::GSplitterSizes&& sizes);

// ----------------------------------------------------------------------------
// GTreeViewHeaderSizes
// ----------------------------------------------------------------------------
#include <QTreeView>
#include <QHeaderView>
namespace GJson {
  struct GTreeViewHeaderSizes {
    GTreeViewHeaderSizes(QTreeView* treeView) { treeView_ = treeView; }
    QTreeView* treeView_;
  };
  GTreeViewHeaderSizes headerSizes(QTreeView* treeView);
}
QJsonValueRef operator << (QJsonValueRef ref, const GJson::GTreeViewHeaderSizes&& headerSizes);
const QJsonValue operator >> (const QJsonValue val, GJson::GTreeViewHeaderSizes&& headerSizes);

#endif // QT_GUI_LIB
