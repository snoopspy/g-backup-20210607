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
#include "gbase.h"

// ----------------------------------------------------------------------------
// GJson
// ----------------------------------------------------------------------------
namespace GJson {
	QJsonObject G_EXPORT loadFromFile(QString fileName = "");
	bool G_EXPORT saveToFile(QJsonObject jo, QString fileName = "");
	QString G_EXPORT defaultFileName();
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
namespace GJson {
	struct GWidgetRect {
		GWidgetRect(QWidget* widget) { widget_ = widget; }
		QWidget* widget_;
	};
	GWidgetRect G_EXPORT rect(QWidget* widget);
}
QJsonValueRef G_EXPORT operator << (QJsonValueRef ref, const GJson::GWidgetRect&& rect);
const QJsonValue G_EXPORT operator >> (const QJsonValue val, GJson::GWidgetRect&& rect);

// ----------------------------------------------------------------------------
// GSplitterSizes
// ----------------------------------------------------------------------------
#include <QSplitter>
namespace GJson {
	struct GSplitterSizes {
		GSplitterSizes(QSplitter* splitter) { splitter_ = splitter; }
		QSplitter* splitter_;
	};
	GSplitterSizes G_EXPORT splitterSizes(QSplitter* splitter);
}
QJsonValueRef G_EXPORT operator << (QJsonValueRef ref, const GJson::GSplitterSizes&& sizes);
const QJsonValue G_EXPORT operator >> (const QJsonValue val, GJson::GSplitterSizes&& sizes);

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
	GTreeViewHeaderSizes G_EXPORT headerSizes(QTreeView* treeView);
}
QJsonValueRef G_EXPORT operator << (QJsonValueRef ref, const GJson::GTreeViewHeaderSizes&& headerSizes);
const QJsonValue G_EXPORT operator >> (const QJsonValue val, GJson::GTreeViewHeaderSizes&& headerSizes);

#endif // QT_GUI_LIB
