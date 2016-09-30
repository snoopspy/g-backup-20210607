#include "gblock.h"

// ----------------------------------------------------------------------------
// GBlock
// ----------------------------------------------------------------------------
void GBlock::block(GPacket* packet) {
  packet->control.block_ = enabled_;
  emit blocked(packet);
}
