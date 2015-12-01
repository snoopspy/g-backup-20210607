#pragma once

#include "person.h"

struct Dept : public GObj {
  Q_OBJECT
  Q_PROPERTY(GObjRef boss READ getBoss)
  Q_PROPERTY(QString deptName MEMBER deptName_)
  Q_PROPERTY(GObjList members MEMBER members_)

  GObjRef getBoss() { return &boss_; }

public:
  Person boss_;
  QString deptName_;
  bool free_{false};
  GObjList members_;
};
