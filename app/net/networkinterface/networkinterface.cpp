#include <GNetworkInterface>

int main()
{
  GNetworkInterfaces& intfs = GNetworkInterface::allInterfaces();

  for (GNetworkInterfaces::iterator it = intfs.begin(); it != intfs.end(); it++) {
    GNetworkInterface& intf = *it;

    // printf("Inteface #%d%s\n", i, intf.name == interfaces.at(0).name ? " (Best)" : ""); // gilgil temp 2015.11.08
    if (intf.name_ != "")
      printf("  name        = %s\n", qPrintable(intf.name_));
    if (intf.description_ !=  "")
      printf("  description = %s\n", qPrintable(intf.description_));

    if (!intf.mac_.isClean())
      printf("  mac         = %s\n", qPrintable(intf.mac_));
    if (intf.ip_ != 0)
      printf("  ip          = %s\n", qPrintable(intf.ip_));
    if (intf.subnet_ != 0)
      printf("  subnet      = %s\n", qPrintable(intf.subnet_));
    if (intf.gateway_ != 0)
      printf("  gateway     = %s\n", qPrintable(intf.gateway_));
    printf("\n");
  }
}
