#include "gnullpacket.h"

// ----------------------------------------------------------------------------
// GNullPacket
// ----------------------------------------------------------------------------
void GNullPacket::parse() {
	qDebug() << "not supported yet"; // gilgil temp 2019.05.25
}

GPacket* GNullPacket::clone() {
	GPacket* res = new GNullPacket;
	res->doClone(this);
	return res;
}
