#include "gpropitem-interface.h"
#include "net/gnetinfo.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemInterface
// ----------------------------------------------------------------------------
GPropItemInterface::GPropItemInterface(GPropItemParam* param) : GPropItemComboBox(param) {
#ifdef Q_OS_ANDROID
	GInterfaceList& interfaceList = GRemoteNetInfo::instance("127.0.0.1", GDemon::DefaultPort).interfaceList();
#else // Q_OS_ANDROID
	GInterfaceList& interfaceList = GNetInfo::instance().interfaceList();
#endif // Q_OS_ANDROID
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

void GPropItemInterface::update() {
	QString intfName = object_->property(mpro_.name()).toString();
#ifdef Q_OS_ANDROID
	GInterfaceList& interfaceList = GRemoteNetInfo::instance("127.0.0.1", GDemon::DefaultPort).interfaceList();
#else // Q_OS_ANDROID
	GInterfaceList& interfaceList = GNetInfo::instance().interfaceList();
#endif // Q_OS_ANDROID
	for (int i = 0; i < interfaceList.count(); i++) {
		const GInterface& intf = interfaceList.at(i);
		if (intf.name() == intfName) {
			comboBox_->setCurrentIndex(i);
			return;
		}
	}
	comboBox_->setCurrentIndex(-1);
	comboBox_->setCurrentText(intfName);
}

void GPropItemInterface::myCurrentIndexChanged(int index) {
	if (index == -1) return;
	QString intfName;
	if (index < intfNames_.count())
		intfName = intfNames_.at(index);
	else
		intfName = comboBox_->currentText();

	bool res = object_->setProperty(mpro_.name(), intfName);
	if (!res) {
		qWarning() << QString("object->setProperty(%1, %2) return false").arg(mpro_.name(), intfName);
	}
}

#endif // QT_GUI_LIB
