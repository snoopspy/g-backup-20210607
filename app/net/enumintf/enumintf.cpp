#include <GNetIntf>
#include <GRtm>

int main() {
  GRtmEntry* entry = GRtm::instance().getBestEntry("8.8.8.8");
  GNetIntfs& intfs = GNetIntf::all();

  for(GNetIntf& intf: intfs) {
    bool best = false;
    if (entry != nullptr && entry->intf()->name() == intf.name()) best = true;
    if (intf.name() != "")
      printf("  name    = %s%s\n", intf.name().toStdString().c_str(), best ? " (Best)" : "");
    if (intf.desc() !=  "")
      printf("  desc    = %s\n", intf.desc().toStdString().c_str());
    if (!intf.mac().isClean())
      printf("  mac     = %s\n", QString(intf.mac()).toStdString().c_str());
    if (intf.ip() != 0)
      printf("  ip      = %s\n", QString(intf.ip()).toStdString().c_str());
    if (intf.mask() != 0)
      printf("  mask    = %s\n", QString(intf.mask()).toStdString().c_str());
    if (intf.gateway() != 0)
      printf("  gateway = %s\n", QString(intf.gateway()).toStdString().c_str());
    printf("\n");
  }
}
