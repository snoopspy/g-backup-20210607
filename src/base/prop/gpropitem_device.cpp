#include "gpropitem_device.h"
#include "net/gnetintf.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemDevice
// ----------------------------------------------------------------------------
GPropItemDevice::GPropItemDevice(GPropItemParam param) : GPropItemComboBox(param) {
  GNetIntfs& intfs = GNetIntf::all();
  for (int i = 0; i < intfs.count(); i++) {
    const GNetIntf& intf = intfs.at(i);
    comboBox_->addItem(intf.name_);
  }
  QObject::connect(comboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(myCurrentIndexChanged(int)));
}

void GPropItemDevice::update() {
  QString dev = object_->property(mpro_.name()).toString();
  GNetIntfs& intfs = GNetIntf::all();
  for (int i = 0; i < intfs.count(); i++) {
    const GNetIntf& intf = intfs.at(i);
    if (intf.name_ == dev) {
      comboBox_->setCurrentIndex(i);
      return;
    }
  }
  comboBox_->setCurrentIndex(-1);
}

void GPropItemDevice::myCurrentIndexChanged(int index) {
  (void)index;
  QString key = comboBox_->currentText();
  bool res = object_->setProperty(mpro_.name(), key);
  if (!res) {
    qWarning() << QString("object->setProperty(%1, %2) return false").arg(mpro_.name(), key);
  }
}

#endif // QT_GUI_LIB
