#include "gblock.h"

// ----------------------------------------------------------------------------
// GBlock
// ----------------------------------------------------------------------------
GBlock::GBlock(QObject* parent) : GStateObj(parent) {
}

GBlock::~GBlock() {
}

void GBlock::block(GPacket* packet) {
	if (!enabled_) return;
	packet->ctrl.block_ = true;
	emit blocked(packet);
}
