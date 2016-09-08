#include "gpcapfile.h"

// ----------------------------------------------------------------------------
// GPcapFile
// ----------------------------------------------------------------------------
GPcapFile::~GPcapFile()  {
  close();
}

bool GPcapFile::doOpen() {
  if (fileName_ == "") {
    SET_ERR(GErr::FILE_NAME_NOT_SPECIFIED, "file name is not specified");
    return false;
  }

  char errBuf[PCAP_ERRBUF_SIZE];
  pcap_ = pcap_open_offline(qPrintable(fileName_), errBuf);
  if (pcap_ == nullptr) {
    SET_ERR(GErr::RETURN_NULL, errBuf);
    return false;
  }

  return GPcap::doOpen();
}

bool GPcapFile::doClose() {
  return GPcap::doClose();
}

// ----------------------------------------------------------------------------
// GTEST
// ----------------------------------------------------------------------------
#ifdef GTEST
#include <gtest/gtest.h>

TEST(GPcapFile, noFileTest) {
  GPcapFile file;
  file.fileName_ = "no_file.pcap";

  ASSERT_FALSE(file.open());
}

TEST(GPcapFile, icmpFileTest) {
  GPcapFile file;
  file.autoRead_ = false;
  file.fileName_ = "icmp8.pcap";

  ASSERT_TRUE(file.open());

  GPacket packet;
  for (int i = 0; i < 8; i++) {
    GCapture::Result res = file.read(&packet);
    ASSERT_TRUE(res == GCapture::Ok);
  }
  GCapture::Result res = file.read(&packet);
  ASSERT_TRUE(res == GCapture::Eof);

  ASSERT_TRUE(file.close());
}

#endif // GTEST
