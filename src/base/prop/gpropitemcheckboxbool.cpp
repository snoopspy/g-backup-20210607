#include "gpropitemcheckboxbool.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemComboBoxBool
// ----------------------------------------------------------------------------
GPropItemComboBoxBool::GPropItemComboBoxBool(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro)
  : GPropItemCheckBox(parent, object, mpro) {
  QObject::connect(checkBox_, SIGNAL(clicked(bool)), this, SLOT(myClicked(bool)));
}

void GPropItemComboBoxBool::update() {
  QVariant value = object_->property(mpro_.name());
  checkBox_->setChecked(value.toBool());
}

void GPropItemComboBoxBool::myClicked(bool checked) {
  bool res = object_->setProperty(mpro_.name(), checked);
  if (!res) {
    qWarning() << QString("object->setProperty(%1, %d) return false").arg(mpro_.name()).arg(checked);
  }
  update();
}

#endif // QT_GUI_LIB
