#include "gblock.h"

// ----------------------------------------------------------------------------
// GBlock
// ----------------------------------------------------------------------------
GBlock::GBlock(QObject* parent) : GStateObj(parent) {
	GDEBUG_CTOR;
}

GBlock::~GBlock() {
	GDEBUG_DTOR;
}

void GBlock::block(GPacket* packet) {
	if (!enabled_) return;
	packet->ctrl.block_ = true;
	emit blocked(packet);
}
