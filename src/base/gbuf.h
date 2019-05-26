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

#include "base/gbase.h"

// ----------------------------------------------------------------------------
// GBuf
// ----------------------------------------------------------------------------
struct G_EXPORT GBuf final {
  unsigned char* data_; // u_char*, gbyte*
  size_t size_;

  GBuf() {}
  GBuf(unsigned char* data, size_t size) : data_(data), size_(size) {}

  void clear() {
    data_ = nullptr;
    size_ = 0;
  }

  bool valid() {
    return data_ != nullptr;
  }
};
