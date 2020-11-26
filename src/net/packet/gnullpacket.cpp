#include "gnullpacket.h"

// ----------------------------------------------------------------------------
// GNullPacket
// ----------------------------------------------------------------------------
void GNullPacket::parse() {
	qDebug() << "not supported yet"; // gilgil temp 2019.05.25
}

GPacket* GNullPacket::clone(size_t extra) {
	GPacket* res = new GNullPacket;
	res->doClone(this, extra);
	return res;
}
