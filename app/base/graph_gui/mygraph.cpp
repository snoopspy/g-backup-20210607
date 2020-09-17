#include "mygraph.h"
#include <QCoreApplication>

// ----------------------------------------------------------------------------
// Block
// ----------------------------------------------------------------------------
#include <GBlock>

void MyFactory::loadBlock() {
	qRegisterMetaType<GBlock*>();

	ItemCategory* category = new ItemCategory("block");
	category->items_.push_back(new ItemNode(GBlock::staticMetaObject.className()));

	items_.push_back(category);
}

// ----------------------------------------------------------------------------
// Capture
// ----------------------------------------------------------------------------
#include <GArpSpoof>
#include <GPcapDevice>
#include <GPcapFile>
#include <GNetFilter>
#include <GWinDivert>

void MyFactory::loadCapture() {
	qRegisterMetaType<GArpSpoof*>();
	qRegisterMetaType<GPcapDevice*>();
	qRegisterMetaType<GPcapFile*>();
#ifdef Q_OS_LINUX
	qRegisterMetaType<GNetFilter*>();
#endif
#ifdef Q_OS_WIN
	qRegisterMetaType<GWinDivert*>();
#endif

	ItemCategory* category = new ItemCategory("capture");
	category->items_.push_back(new ItemNode(GArpSpoof::staticMetaObject.className()));
	category->items_.push_back(new ItemNode(GPcapDevice::staticMetaObject.className()));
	category->items_.push_back(new ItemNode(GPcapFile::staticMetaObject.className()));
#ifdef Q_OS_LINUX
	category->items_.push_back(new ItemNode(GNetFilter::staticMetaObject.className()));
#endif
#ifdef Q_OS_WIN
	category->items_.push_back(new ItemNode(GWinDivert::staticMetaObject.className()));
#endif

	items_.push_back(category);
}

// ----------------------------------------------------------------------------
// Delay
// ----------------------------------------------------------------------------
#include <GDelay>
#include <GSyncDelay>
#include <GTimeStampSyncDelay>

void MyFactory::loadDelay() {
	qRegisterMetaType<GBlock*>();
	qRegisterMetaType<GSyncDelay*>();
	qRegisterMetaType<GTimeStampSyncDelay*>();

	ItemCategory* category = new ItemCategory("block");
	category->items_.push_back(new ItemNode(GDelay::staticMetaObject.className()));
	category->items_.push_back(new ItemNode(GSyncDelay::staticMetaObject.className()));
	category->items_.push_back(new ItemNode(GTimeStampSyncDelay::staticMetaObject.className()));

	items_.push_back(category);
}

// ----------------------------------------------------------------------------
// Extract
// ----------------------------------------------------------------------------
#include <GHttpExtract>

void MyFactory::loadExtract() {
	qRegisterMetaType<GHttpExtract*>();

	ItemCategory* category = new ItemCategory("extract");
	category->items_.push_back(new ItemNode(GHttpExtract::staticMetaObject.className()));

	items_.push_back(category);
}

// ----------------------------------------------------------------------------
// Filter
// ----------------------------------------------------------------------------
#include <GBpFilter>

void MyFactory::loadFilter() {
	qRegisterMetaType<GBpFilter*>();

	ItemCategory* category = new ItemCategory("filter");
	category->items_.push_back(new ItemNode(GBpFilter::staticMetaObject.className()));

	items_.push_back(category);
}

// ----------------------------------------------------------------------------
// Flow
// ----------------------------------------------------------------------------
#include <GIpFlowMgr>
#include <GTcpFlowMgr>
#include <GUdpFlowMgr>

void MyFactory::loadFlow() {
	qRegisterMetaType<GIpFlowMgr*>();
	qRegisterMetaType<GTcpFlowMgr*>();
	qRegisterMetaType<GUdpFlowMgr*>();

	ItemCategory* category = new ItemCategory("flow");
	category->items_.push_back(new ItemNode(GIpFlowMgr::staticMetaObject.className()));
	category->items_.push_back(new ItemNode(GTcpFlowMgr::staticMetaObject.className()));
	category->items_.push_back(new ItemNode(GUdpFlowMgr::staticMetaObject.className()));

	items_.push_back(category);
}

// ----------------------------------------------------------------------------
// Other
// ----------------------------------------------------------------------------
#include <GCommand>

void MyFactory::loadOther() {
	qRegisterMetaType<GCommand*>();

	ItemCategory* category = new ItemCategory("other");
	category->items_.push_back(new ItemNode(GCommand::staticMetaObject.className()));

	items_.push_back(category);
}

// ----------------------------------------------------------------------------
// Process
// ----------------------------------------------------------------------------
#include <GBypassSslBlock>
#include <GCorrectChecksum>
#include <GDnsProcessor>
#include <GFlowMgrDebugger>
#include <GPacketDebugger>

void MyFactory::loadProcess() {
	qRegisterMetaType<GBypassSslBlock*>();
	qRegisterMetaType<GCorrectChecksum*>();
	qRegisterMetaType<GDnsProcessor*>();
	qRegisterMetaType<GFlowMgrDebugger*>();
	qRegisterMetaType<GPacketDebugger*>();

	ItemCategory* category = new ItemCategory("process");
	category->items_.push_back(new ItemNode(GBypassSslBlock::staticMetaObject.className()));
	category->items_.push_back(new ItemNode(GCorrectChecksum::staticMetaObject.className()));
	category->items_.push_back(new ItemNode(GDnsProcessor::staticMetaObject.className()));
	category->items_.push_back(new ItemNode(GFlowMgrDebugger::staticMetaObject.className()));
	category->items_.push_back(new ItemNode(GPacketDebugger::staticMetaObject.className()));

	items_.push_back(category);
}

// ----------------------------------------------------------------------------
// Write
// ----------------------------------------------------------------------------
#include <GPcapDeviceWrite>
#include <GPcapDeviceWriteEth>
#include <GPcapDeviceWriteEthAutoMac>
#include <GPcapDeviceWriteIp>
#include <GPcapFileWrite>
#include <GPcapFileWriteEth>
#include <GPcapFileWriteIp>

void MyFactory::loadWrite() {
	qRegisterMetaType<GPcapDeviceWrite*>();
	qRegisterMetaType<GPcapDeviceWriteEth*>();
	qRegisterMetaType<GPcapDeviceWriteEthAutoMac*>();
	qRegisterMetaType<GPcapDeviceWriteIp*>();
	qRegisterMetaType<GPcapFileWrite*>();
	qRegisterMetaType<GPcapFileWriteEth*>();
	qRegisterMetaType<GPcapFileWriteIp*>();

	ItemCategory* category = new ItemCategory("other");
	category->items_.push_back(new ItemNode(GPcapDeviceWrite::staticMetaObject.className()));
	category->items_.push_back(new ItemNode(GPcapDeviceWriteEth::staticMetaObject.className()));
	category->items_.push_back(new ItemNode(GPcapDeviceWriteEthAutoMac::staticMetaObject.className()));
	category->items_.push_back(new ItemNode(GPcapDeviceWriteIp::staticMetaObject.className()));
	category->items_.push_back(new ItemNode(GPcapFileWrite::staticMetaObject.className()));
	category->items_.push_back(new ItemNode(GPcapFileWriteEth::staticMetaObject.className()));
	category->items_.push_back(new ItemNode(GPcapFileWriteIp::staticMetaObject.className()));

	items_.push_back(category);
}


MyFactory::MyFactory(QObject* parent) : GPluginFactory(parent) {
	loadBlock();
	loadCapture();
	loadDelay();
	loadExtract();
	loadFilter();
	loadFlow();
	loadOther();
	loadProcess();
	loadWrite();
	load("plugin");
}

MyFactory::~MyFactory() {
}
