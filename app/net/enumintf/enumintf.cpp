#include <GNetIntf>
#include <GRtm>

int main() {
  GRtmEntry* entry = GRtm::instance().getBestEntry("8.8.8.8");
  GNetIntfs& intfs = GNetIntf::all();

  for (int i = 0; i < intfs.count(); i++) {
    const GNetIntf& intf = intfs.at(i);

    bool best = false;
    if (entry != nullptr && entry->intf_ == intf.name_) best = true;

    if (intf.name_ != "")
      printf("  name        = %s%s\n", qPrintable(intf.name_), best ? " (Best)" : "");
    if (intf.description_ !=  "")
      printf("  description = %s\n", qPrintable(intf.description_));
    if (!intf.mac_.isClean())
      printf("  mac         = %s\n", qPrintable(intf.mac_));
    if (intf.ip_ != 0)
      printf("  ip          = %s\n", qPrintable(intf.ip_));
    if (intf.mask_ != 0)
      printf("  mask        = %s\n", qPrintable(intf.mask_));
    if (intf.gateway_ != 0)
      printf("  gateway     = %s\n", qPrintable(intf.gateway_));
    printf("\n");
  }
}
