#ifdef QT_GUI_LIB

#include <QGridLayout>
#include <QHeaderView>
#include "gpropitemlineeditobjectname.h"
#include "gpropwidget.h"

// ----------------------------------------------------------------------------
// GPropWidget
// ----------------------------------------------------------------------------
GPropWidget::GPropWidget(QWidget *parent) : QTreeWidget(parent) {
  init();
}

GPropWidget::GPropWidget(GObj* obj) : QTreeWidget(nullptr) {
  init();
  setObject(obj);
}

GPropWidget::~GPropWidget() {
}

void GPropWidget::init() {
  this->setColumnCount(2);
  this->setHeaderLabels(QStringList() << "property" << "value");
  QLayout* layout = new QGridLayout(this);
  layout->setMargin(0);
  obj_ = nullptr;
}

QObject* GPropWidget::object() {
  return obj_;
}

void GPropWidget::setObject(GObj* obj) {
  if (obj == obj_) return;
  clear();
  obj_ = obj;

  QMetaProperty mpro = obj->metaObject()->property(0); // objectName
  GPropItemLineEditObjectName* root = new GPropItemLineEditObjectName(this, obj, mpro);
  QTreeWidgetItem* item = root->item_;
  obj_->createPropItems(item);

  update();
  expandAll();

  if (isFirstSetObject_) {
    isFirstSetObject_ = false;
    resizeColumnToContents(0);
  }
}

void GPropWidget::update() {
  QObjectList list = children();
  foreach (QObject* object, list) {
    GPropItem* item = dynamic_cast<GPropItem*>(object);
    if (item != nullptr)
      item->update();
  }
}

#endif // QT_GUI_LIB
