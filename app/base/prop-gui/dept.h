#pragma once

#include "person.h"

struct G_EXPORT Dept : public GObj {
	Q_OBJECT
	Q_PROPERTY(GObjRef boss READ getBoss)
	Q_PROPERTY(QString deptName MEMBER deptName_)
	Q_PROPERTY(GObjRefArrayPtr members READ getMembers)

public:
	GObjRef getBoss() { return &boss_; }
	GObjRefArrayPtr getMembers() { return &members_; }

public:
	Person boss_;
	QString deptName_;
	bool free_{false};
	GObjRefArray<Person> members_;
};
