// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include "net/write/gpcapdevicewrite.h"

// ----------------------------------------------------------------------------
// GTcpBlock
// ----------------------------------------------------------------------------
struct G_EXPORT GTcpBlock : GPcapDeviceWrite {
	Q_OBJECT
	Q_PROPERTY(bool enabled MEMBER enabled_)
	Q_PROPERTY(bool forwardRst MEMBER forwardRst_)
	Q_PROPERTY(bool backwardRst MEMBER backwardRst_)
	Q_PROPERTY(bool forwardFin MEMBER forwardFin_)
	Q_PROPERTY(bool backwardFin MEMBER backwardFin_)
	Q_PROPERTY(QStringList forwardFinMsg MEMBER forwardFinMsg_)
	Q_PROPERTY(QStringList backwardFinMsg MEMBER backwardFinMsg_)

public:
	bool enabled_{true};
	bool forwardRst_{true};
	bool backwardRst_{true};
	bool forwardFin_{false};
	bool backwardFin_{false};
	QStringList forwardFinMsg_;
	QStringList backwardFinMsg_;

public:
	Q_INVOKABLE GTcpBlock(QObject* parent = nullptr);
	~GTcpBlock() override;

protected:
	bool doOpen() override;
	bool doClose() override;

protected:
	QString forwardFinMsgStr_;
	QString backwardFinMsgStr_;
	int maxMsgSize_{0};

public slots:
	void block(GPacket* packet);

signals:
	void blocked(GPacket* packet);
};
