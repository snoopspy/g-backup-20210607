#include "gpdu.h"

// ----------------------------------------------------------------------------
// GPdu
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// GPdus
// ----------------------------------------------------------------------------
void GPdus::clear() {
   current_ = 0;

  int count = size();
  for (int i = count - 1; i >= 0; i--) {
    GPdu* pdu = this->at(i);
    delete pdu;
  }
  QVector<GPdu*>::clear();
}

void GPdus::add(GPdu* pdu) {
  push_back(pdu);
}

GPdu* GPdus::first() {
  current_ = 0;
  if (size() > 0)
    return at(0);
  return nullptr;
}

GPdu* GPdus::next() {
  Q_ASSERT(current_ >= 0 && current_ < size());
  current_++;
  if (current_ >= 0 && current_ < size())
    return at(current_);
  return nullptr;
}

GPdu* GPdus::prev() {
  Q_ASSERT(current_ >= 0 && current_ < size());
  current_--;
  if (current_ >= 0 && current_ < size())
    return at(current_);
  return nullptr;
}

GPdu* GPdus::findFirst(GPdu::Type type) {
  int count = size();
  for (int i = 0; i < count; i++) {
    GPdu* pdu = at(i);
    if (pdu->type() == type) {
      current_ = i;
      return pdu;
    }
  }
  return nullptr;
}

GPdu* GPdus::findNext(GPdu::Type type) {
  Q_ASSERT(current_ >= 0 && current_ < size());
  int count = size();
  for (int i = current_ + 1; i < count; i++) {
    GPdu* pdu = at(i);
    if (pdu->type() == type) {
      current_ = i;
      return pdu;
    }
  }
  return nullptr;
}

GPdu* GPdus::findPrev(GPdu::Type type) {
  Q_ASSERT(current_ >= 0 && current_ < size());
  for (int i = current_ - 1; i >= 0; i--) {
    GPdu* pdu = at(i);
    if (pdu->type() == type) {
      current_ = i;
      return pdu;
    }
  }
  return nullptr;
}
