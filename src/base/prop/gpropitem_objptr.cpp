#include "gpropitem_objptr.h"
#include <QMetaEnum> // gilgil temp 2016.10.10

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemObjPtr
// ----------------------------------------------------------------------------
GPropItemObjPtr::GPropItemObjPtr(GPropItemParam param) : GPropItemComboBox(param) {
  QMetaEnum menum = param.mpro_.enumerator();
  int count = menum.keyCount();
  for (int i = 0; i < count; i++) {
    comboBox_->addItem(menum.key(i));
  }
  QObject::connect(comboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(myCurrentIndexChanged(int)));
}

void GPropItemObjPtr::update() {
  QMetaEnum menum = mpro_.enumerator();
  QString key = menum.valueToKey(object_->property(mpro_.name()).toInt());
  comboBox_->setCurrentText(key);
}

void GPropItemObjPtr::myCurrentIndexChanged(int index) {
  (void)index;
  QString key = comboBox_->currentText();
  bool res = object_->setProperty(mpro_.name(), key);
  if (!res) {
    qWarning() << QString("object->setProperty(%1, %2) return false").arg(mpro_.name(), key);
  }
}

#endif // QT_GUI_LIB
