#include "gpropitem-device.h"
#include "net/gnetinfo.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemDevice
// ----------------------------------------------------------------------------
GPropItemDevice::GPropItemDevice(GPropItemParam* param) : GPropItemComboBox(param) {
	GInterfaceList& interfaceList = GNetInfo::instance().interfaceList();
	for (int i = 0; i < interfaceList.count(); i++) {
		const GInterface& intf = interfaceList.at(i);
#ifdef Q_OS_LINUX
		QString s = intf.name();
#endif
#ifdef Q_OS_WIN
		QString s = intf.desc();
#endif
		comboBox_->addItem(s);
		intfNames_.push_back(intf.name());
	}
	QObject::connect(comboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(myCurrentIndexChanged(int)));
}

void GPropItemDevice::update() {
	QString intfName = object_->property(mpro_.name()).toString();
	GInterfaceList& interfaceList = GNetInfo::instance().interfaceList();
	for (int i = 0; i < interfaceList.count(); i++) {
		const GInterface& intf = interfaceList.at(i);
		if (intf.name() == intfName) {
			comboBox_->setCurrentIndex(i);
			return;
		}
	}
	comboBox_->setCurrentIndex(-1);
}

void GPropItemDevice::myCurrentIndexChanged(int index) {
	if (index == -1) return;
	Q_ASSERT(index <= intfNames_.count());
	QString intfName = intfNames_.at(index);
	bool res = object_->setProperty(mpro_.name(), intfName);
	if (!res) {
		qWarning() << QString("object->setProperty(%1, %2) return false").arg(mpro_.name(), intfName);
	}
}

#endif // QT_GUI_LIB
