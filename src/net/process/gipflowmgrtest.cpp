#include "gipflowmgrtest.h"

// ----------------------------------------------------------------------------
// GIpFlowMgrTest
// ----------------------------------------------------------------------------
void GIpFlowMgrTest::test(GPacket* packet) {
  qDebug() << "size=" << packet->buf_.size_; // gilgil temp 2016.10.10

  emit tested(packet);
}
