#include <QCoreApplication>
#include <QDir>
#include <QJsonDocument>
#include "gjson.h"

// ----------------------------------------------------------------------------
// GJson
// ----------------------------------------------------------------------------
QJsonObject GJson::loadFromFile(QString fileName) {
  if (fileName == "")
    fileName = fileName_;
  if (fileName == "")
    fileName = defaultFileName();
  fileName_ = fileName;

  if (!QFile::exists(fileName)) {
    return QJsonObject();
  }
  QFile file(fileName);
  if (!file.open(QFile::ReadOnly)) {
    qWarning() << QString("file open(%1) failed(%2)").arg(fileName, file.errorString());
    return QJsonObject();
  }
  QByteArray ba = file.readAll();
  QJsonDocument doc = QJsonDocument::fromJson(ba);

  return doc.object();
}

void GJson::saveToFile(QJsonObject jo, QString fileName) {
  if (fileName == "")
    fileName = fileName_;
  if (fileName == "")
    fileName = defaultFileName();
  fileName_ = fileName;

  QFile file(fileName);
  if (!file.open(QFile::WriteOnly)) {
    qWarning() << QString("file open(%1) failed(%2)").arg(fileName, file.errorString());
    return;
  }
  QJsonDocument doc;
  doc.setObject(jo);
  file.write(doc.toJson());
}

GJson& GJson::instance() {
  static GJson json;
  return json;
}

QString GJson::defaultFileName() {
  QString res;
  res = QCoreApplication::applicationFilePath();
  res += ".json";
  return res;
}


// ----------------------------------------------------------------------------
// QList<int>
// ----------------------------------------------------------------------------
QJsonValueRef operator << (QJsonValueRef ref, const QList<int>& intList) {
  QStringList strList;
  for(int i: intList) {
    QString s = QString::number(i);
    strList.append(s);
  }

  ref = strList.join(',');
  return ref;
}

QJsonValueRef operator >> (const QJsonValueRef ref, QList<int>& intList) {
  QString s = ref.toString();

  if (s != "") {
    QStringList strList = ref.toString().split(',');
    for(QString s: strList) {
      intList.append(s.toInt());
    }
  }

  return ref;
}

#include "gobj.h"

// ----------------------------------------------------------------------------
// GObj
// ----------------------------------------------------------------------------
QJsonValueRef operator << (QJsonValueRef ref, const GObj& obj) {
  QJsonObject jo;

  ((GObj&)obj).save(jo);

  ref = jo;
  return ref;
}

QJsonValueRef operator >> (const QJsonValueRef ref, GObj& obj) {
  QJsonObject jo = ref.toObject();

  if (!jo.empty()) {
    obj.load(jo);
  }

  return ref;
}

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// QSplitter
// ----------------------------------------------------------------------------
QJsonValueRef operator << (QJsonValueRef ref, const QSplitter* splitter) {
  ref << splitter->sizes();
  return ref;
}

QJsonValueRef operator >> (const QJsonValueRef ref, QSplitter* splitter) {
  QList<int> sizes;
  ref >> sizes;
  if (sizes.count() != 0)
    splitter->setSizes(sizes);
  return ref;
}

// ----------------------------------------------------------------------------
// QWidget
// ----------------------------------------------------------------------------
#include <QPoint>
#include <QSize>
QJsonObject operator << (QJsonObject& jo, const QWidget* widget) {
  QPoint pos = widget->pos();
  jo["left"] = pos.x();
  jo["top"] = pos.y();

  QSize size = widget->size();
  jo["width"] = size.width();
  jo["height"] = size.height();

  return jo;
}

QJsonObject operator >> (const QJsonObject jo, QWidget* widget) {
  if (!jo.isEmpty()) {
    QPoint pos;
    pos.setX(jo["left"].toInt());
    pos.setY(jo["top"].toInt());
    widget->move(pos);

    QSize size;
    size.setWidth(jo["width"].toInt());
    size.setHeight(jo["height"].toInt());
    widget->resize(size);
  }

  return jo;
}

QJsonValueRef operator << (QJsonValueRef ref, const QWidget* widget) {
  QJsonObject jo;
  jo << widget;
  ref = jo;
  return ref;
}

QJsonValueRef operator >> (const QJsonValueRef ref, QWidget* widget) {
  QJsonObject jo = ref.toObject();
  jo >> widget;
  return ref;
}

// ----------------------------------------------------------------------------
// QTreeView
// ----------------------------------------------------------------------------
QJsonValueRef operator << (QJsonValueRef ref, const QTreeView* treeView) {
  int count = treeView->header()->count() - 1;
  QList<int> sizes;
  for (int i = 0; i < count; i++) {
    sizes << treeView->columnWidth(i);
  }
  ref << sizes;
  return ref;
}

QJsonValueRef operator >> (const QJsonValueRef ref, QTreeView* treeView) {
  QList<int> sizes;
  ref >> sizes;
  if (sizes.count() != 0) {
    int count = treeView->header()->count();
    for (int i = 0; i < count - 1; i++) {
      treeView->setColumnWidth(i, sizes.at(i));
    }
  }
  return ref;
}

#endif // QT_GUI_LIB
