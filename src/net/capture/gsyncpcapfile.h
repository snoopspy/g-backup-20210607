#include "net/capture/gpcapfile.h"

// ----------------------------------------------------------------------------
// GSyncPcapFile
// ----------------------------------------------------------------------------
struct GSyncPcapFile : GPcapFile {
  Q_OBJECT

public:
  Q_INVOKABLE GSyncPcapFile(QObject* parent = nullptr) : GPcapFile(parent) {
    autoRead_ = false;
  }
};
