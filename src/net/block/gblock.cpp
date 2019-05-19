#include "gblock.h"

// ----------------------------------------------------------------------------
// GBlock
// ----------------------------------------------------------------------------
GBlock::GBlock(QObject* parent) : GStateObj(parent) {
  qDebug() << "GBlock::GBlock" << pvoid(this); // gilgil temp 2019.05.19
}

GBlock::~GBlock() {
  qDebug() << "GBlock::~GBlock" << pvoid(this); // gilgil temp 2019.05.19
}

void GBlock::block(GPacket* packet) {
  if (!enabled_) return;
  packet->ctrl.block_ = true;
  emit blocked(packet);
}
