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
#include <QJsonDocument>
#include <QJsonObject>

// ----------------------------------------------------------------------------
// GJson
// ----------------------------------------------------------------------------
struct GJson {
  bool loadFromFile(QString fileName = "");
  bool saveToFile(QString fileName = "");
  static GJson& instance();

public:
  QJsonDocument doc_;
  QJsonObject jo_;

protected:
  static QString defaultFileName();

  QString fileName_;
};

#include <QList>

// ----------------------------------------------------------------------------
// QList<int>
// ----------------------------------------------------------------------------
QJsonValueRef operator << (QJsonValueRef ref, const QList<int>& intList);
QJsonValueRef operator >> (const QJsonValueRef ref, QList<int>& intList);

// ----------------------------------------------------------------------------
// GObj
// ----------------------------------------------------------------------------
class GObj;
QJsonValueRef operator << (QJsonValueRef ref, const GObj& obj);
QJsonValueRef operator >> (const QJsonValueRef ref, GObj& obj);

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// QSplitter
// ----------------------------------------------------------------------------
#include <QSplitter>
QJsonValueRef operator << (QJsonValueRef ref, const QSplitter* splitter);
QJsonValueRef operator >> (const QJsonValueRef ref, QSplitter* splitter);

// ----------------------------------------------------------------------------
// QWidget
// ----------------------------------------------------------------------------
#include <QWidget>
//QJsonObject operator << (QJsonObject& jo, const QWidget* widget); // gilgil temp 2015.11.20
//QJsonObject operator >> (const QJsonObject jo, QWidget* widget); // gilgil temp 2015.11.20
QJsonValueRef operator << (QJsonValueRef ref, const QWidget* widget);
QJsonValueRef operator >> (const QJsonValueRef ref, QWidget* widget);

// ----------------------------------------------------------------------------
// QTreeView
// ----------------------------------------------------------------------------
#include <QTreeView>
#include <QHeaderView>
QJsonValueRef operator << (QJsonValueRef ref, const QTreeView* treeView);
QJsonValueRef operator >> (const QJsonValueRef ref, QTreeView* treeView);

#endif // QT_GUI_LIB
