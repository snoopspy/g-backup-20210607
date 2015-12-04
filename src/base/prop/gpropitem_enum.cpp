#include "gpropitem_enum.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemEnum
// ----------------------------------------------------------------------------
void GPropItemEnum::init(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, QObject* object, QMetaProperty mpro) {
  GPropItemComboBox::init(treeWidget, parentItem, object, mpro);
  QMetaEnum menum = mpro.enumerator();
  int count = menum.keyCount();
  for (int i = 0; i < count; i++) {
    comboBox_->addItem(menum.key(i));
  }
  QObject::connect(comboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(myCurrentIndexChanged(int)));
}

void GPropItemEnum::update() {
  QMetaEnum menum = mpro_.enumerator();
  QString key = menum.valueToKey(object_->property(mpro_.name()).toInt());
  comboBox_->setCurrentText(key);
}

void GPropItemEnum::myCurrentIndexChanged(int index) {
  (void)index;
  QString key = comboBox_->currentText();
  bool res = object_->setProperty(mpro_.name(), key);
  if (!res) {
    qWarning() << QString("object->setProperty(%1, %2) return false").arg(mpro_.name(), key);
  }
}

#endif // QT_GUI_LIB

