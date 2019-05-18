#include <GNetIntf>
#include <GRtm>

int main() {
  GRtmEntry* entry = GRtm::instance().getBestEntry("8.8.8.8");
  GNetIntfs& intfs = GNetIntf::all();

  for (int i = 0; i < intfs.count(); i++) {
    const GNetIntf& intf = intfs.at(i);

    bool best = false;
    if (entry != nullptr && entry->intf_->name_ == intf.name_) best = true;
    if (intf.name_ != "")
      printf("  name        = %s%s\n", intf.name_.toStdString().c_str(), best ? " (Best)" : "");
    if (intf.description_ !=  "")
      printf("  description = %s\n", intf.description_.toStdString().c_str());
    if (!intf.mac_.isClean())
      printf("  mac         = %s\n", QString(intf.mac_).toStdString().c_str());
    if (intf.ip_ != 0)
      printf("  ip          = %s\n", QString(intf.ip_).toStdString().c_str());
    if (intf.mask_ != 0)
      printf("  mask        = %s\n", QString(intf.mask_).toStdString().c_str());
    if (intf.gateway_ != 0)
      printf("  gateway     = %s\n", QString(intf.gateway_).toStdString().c_str());
    printf("\n");
  }
}
