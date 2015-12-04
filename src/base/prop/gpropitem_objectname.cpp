#include "gpropitem_objectname.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemObjectName
// ----------------------------------------------------------------------------
void GPropItemObjectName::init(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, QObject* object, QMetaProperty mpro) {
  GPropItemLineEdit::init(treeWidget, parentItem, object, mpro);
  item_->setText(0, object->metaObject()->className());
  QObject::connect(lineEdit_, SIGNAL(editingFinished()), this, SLOT(myEditingFinished()));
}

void GPropItemObjectName::update() {
  QVariant value = object_->property("objectName");
  lineEdit_->setText(value.toString());
}

void GPropItemObjectName::myEditingFinished() {
  bool res = object_->setProperty("objectName", lineEdit_->text());
  if (!res) {
    qWarning() << QString("object->setProperty(%1, %2) return false").arg("objectName", lineEdit_->text());
  }
  update();
}

#endif // QT_GUI_LIB
