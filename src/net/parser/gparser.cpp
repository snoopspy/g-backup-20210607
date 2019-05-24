#include "gparser.h"
#include "net/packet/gpacket.h"
#include "net/pdu/gpdu.h"

// ----------------------------------------------------------------------------
// GParser
// ----------------------------------------------------------------------------
void GParser::parse(GPacket* packet) {
  (void)packet;
  SET_ERR(GErr::VIRTUAL_FUNCTION_CALL, "virtual function call");
}
