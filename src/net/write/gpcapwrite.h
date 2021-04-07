// ----------------------------------------------------------------------------
//
// G Library
//
// http://gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include "net/write/gwrite.h"

// ----------------------------------------------------------------------------
// GPcapWrite
// ----------------------------------------------------------------------------
struct G_EXPORT GPcapWrite : GWrite {
	Q_OBJECT

public:
	Q_INVOKABLE GPcapWrite(QObject* parent = nullptr) : GWrite(parent) {}
	~GPcapWrite() override {}

protected:
	pcap_t* pcap_{nullptr};
};
