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

#include <QVector>

// ----------------------------------------------------------------------------
// GFlow
// ----------------------------------------------------------------------------
namespace GFlow {
  // --------------------------------------------------------------------------
  // Value
  // --------------------------------------------------------------------------
  struct Value {
    enum State {
      Half,
      Full,
      Rst,
      Fin
    };
    struct timeval ts_;
    State state_;
    u_char totalMem_[0];

    static struct Value* allocate(struct timeval ts, State state, size_t totalMemSize) {
      Value* res = (Value*)malloc(sizeof(struct Value) + totalMemSize);
      res->ts_ = ts;
      res->state_ = state;
      return res;
    }

    static void deallocate(Value* value) {
      free((void*)value);
    }

    u_char* mem(size_t offset) { return totalMem_ + offset; }
  };

  // ----------------------------------------------------------------------------
  // RequestItem
  // ----------------------------------------------------------------------------
  struct RequestItem {
    void* id_{nullptr};
    size_t offset_{0};
    size_t memSize_{0};
  };

  // ----------------------------------------------------------------------------
  // RequestItems
  // ----------------------------------------------------------------------------
  struct RequestItems : QVector<RequestItem> {
    size_t totalMemSize_{0};

    size_t request(void* id, size_t memSize) {
      size_t currentOffset = 0;
      foreach (const RequestItem& item, *this) {
        if (item.id_ == id) return currentOffset;
        currentOffset += item.memSize_;
      }

      RequestItem newItem;
      newItem.id_ = id;
      newItem.offset_ = currentOffset;
      newItem.memSize_ = memSize;

      push_back(newItem);
      totalMemSize_ += memSize;

      return currentOffset;
    }
  };
}


