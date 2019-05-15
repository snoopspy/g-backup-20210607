// ----------------------------------------------------------------------------
//
// G Library
//
// http://www.gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#include <QtCore>

#ifdef Q_OS_WIN
  #include <winsock2.h> // for u_char
#endif

// ----------------------------------------------------------------------------
// GBuf
// ----------------------------------------------------------------------------
struct GBuf final {
  u_char* data_;
  size_t size_;

  void clear() {
    data_ = nullptr;
    size_ = 0;
  }

  bool valid() {
    return data_  != nullptr;
  }
};
