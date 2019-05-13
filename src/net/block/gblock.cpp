#include "gblock.h"

// ----------------------------------------------------------------------------
// GBlock
// ----------------------------------------------------------------------------
void GBlock::block(GPacket* packet) {
  if (!enabled_) return;
  packet->ctrl.block_ = true;
  emit blocked(packet);
}
