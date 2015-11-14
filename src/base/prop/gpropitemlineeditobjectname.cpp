#include "gpropitemlineeditobjectname.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemLineEditObjectName
// ----------------------------------------------------------------------------
GPropItemLineEditObjectName::GPropItemLineEditObjectName(QTreeWidget* widget, QObject* object, QMetaProperty mpro)
  : GPropItemLineEdit(widget, object, mpro) {
  item_->setText(0, object->metaObject()->className());
  QObject::connect(lineEdit_, SIGNAL(editingFinished()), this, SLOT(myEditingFinished()));
}

GPropItemLineEditObjectName::GPropItemLineEditObjectName(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro)
  : GPropItemLineEdit(parent, object, mpro) {
  QObject::connect(lineEdit_, SIGNAL(editingFinished()), this, SLOT(myEditingFinished()));
}

void GPropItemLineEditObjectName::update() {
  QVariant value = object_->property("objectName");
  lineEdit_->setText(value.toString());
}

void GPropItemLineEditObjectName::myEditingFinished() {
  bool res = object_->setProperty("objectName", lineEdit_->text());
  if (!res) {
    qWarning() << QString("object->setProperty(%1, %2) return false").arg("objectName", lineEdit_->text());
  }
  update();
}

#endif // QT_GUI_LIB
