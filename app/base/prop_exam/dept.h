#pragma once

#include "person.h"

struct Dept : public GObj {
  Q_OBJECT
  Q_PROPERTY(GObjPtr boss READ getBoss)
  Q_PROPERTY(QString deptName MEMBER deptName_)
  Q_PROPERTY(GObjPtrListPtr members READ getMembers)

  GObjPtr getBoss() { return &boss_; }
  GObjPtrListPtr getMembers() { return &members_; }

public:
  Person boss_;
  QString deptName_;
  bool free_{false};
  GObjPtrList<Person> members_;
};
