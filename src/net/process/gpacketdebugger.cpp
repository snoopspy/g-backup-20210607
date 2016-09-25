#include "gpacketdebugger.h"

// ----------------------------------------------------------------------------
// GPacketDebugger
// ----------------------------------------------------------------------------
bool GPacketDebugger::doOpen() {
  return true;
}

bool GPacketDebugger::doClose() {
  return true;
}

void GPacketDebugger::debug(GPacket* packet) {
  qDebug() << QString("captured %1").arg(packet->pkthdr_.len);
}
