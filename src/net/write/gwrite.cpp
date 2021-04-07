#include "gwrite.h"

// ----------------------------------------------------------------------------
// GWrite
// ----------------------------------------------------------------------------
GPacket::Result GWrite::write(GPacket* packet) {
	(void)packet;
	SET_ERR(GErr::NOT_SUPPORTED, "not supported");
	return GPacket::Fail;
}

GPacket::Result GWrite::write(GBuf buf) {
	(void)buf;
	SET_ERR(GErr::NOT_SUPPORTED, "not supported");
	return GPacket::Fail;
}
