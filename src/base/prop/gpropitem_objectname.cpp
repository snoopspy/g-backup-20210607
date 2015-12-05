#include "gpropitem_objectname.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemObjectName
// ----------------------------------------------------------------------------
void GPropItemObjectName::init(GPropItemParam param) {
  GPropItemLineEdit::init(param);
  item_->setText(0, param.object_->metaObject()->className());
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
