#ifdef QT_GUI_LIB

#include "gpropwidget.h"

#include <QGridLayout>
#include <QHeaderView>
#include "base/gjson.h"

// ----------------------------------------------------------------------------
// GPropWidget
// ----------------------------------------------------------------------------
GPropWidget::GPropWidget(QWidget *parent) : QTreeWidget(parent) {
  init();
}

GPropWidget::GPropWidget(QObject* object) : QTreeWidget(nullptr) {
  init();
  setObject(object);
}

GPropWidget::~GPropWidget() {
  clear();
}

void GPropWidget::init() {
  this->setIndentation(12);
  this->setColumnCount(2);
  this->setHeaderLabels(QStringList() << "property" << "value");
  QLayout* layout = new QGridLayout(this);
  layout->setMargin(0);
  object_ = nullptr;
}

void GPropWidget::setObject(QObject* object) {
  if (object == object_) return;
  clear();
  object_ = object;

  if (object == nullptr) return;

  GProp* prop = dynamic_cast<GProp*>(object_);
  if (prop == nullptr) {
    qWarning() << "prop is nullptr. object must be descendant of both QObject and GProp";
    return;
  }
  prop->propCreateItems(this, nullptr, object_);

  update();

  if (isFirstSetObject_) {
    isFirstSetObject_ = false;
    resizeColumnToContents(0);
  }
}

void GPropWidget::update() {
  QObjectList list = children();
  for (QObject* object: list) {
    GPropItem* item = dynamic_cast<GPropItem*>(object);
    if (item != nullptr)
      item->update();
  }
  QTreeWidget::update();
}

void GPropWidget::clear() {
  QObjectList list = children();
  for (QObject* object: list) {
    GPropItem* item = dynamic_cast<GPropItem*>(object);
    if (item != nullptr)
      delete item;
  }
  QTreeWidget::clear();
}

void GPropWidget::propLoad(QJsonObject jo) {
  jo["rect"] >> GJson::rect(this);
  jo["sizes"] >> GJson::headerSizes(this);
}

void GPropWidget::propSave(QJsonObject& jo) {
  jo["rect"] << GJson::rect(this);
  jo["sizes"] << GJson::headerSizes(this);
}

#endif // QT_GUI_LIB
