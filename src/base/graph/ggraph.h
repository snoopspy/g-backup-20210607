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

#include <QList>
#include "base/gobj.h"

// ----------------------------------------------------------------------------
// GGraph
// ----------------------------------------------------------------------------
struct GGraph : GObj {
  struct Node : QObject {
  };

  struct Nodes : QList<Node*>, GProp {
    void propLoad(QJsonObject jo) override;
    void propSave(QJsonObject& jo) override;
  };

  struct Connection {
    QString sender_;
    QString signal_;
    QString receiver_;
    QString slot_;
  };

  struct Connections : QList<Connection*>, GProp {
    void propLoad(QJsonObject jo) override;
    void propSave(QJsonObject& jo) override;
  };

  struct Factory {
    struct Item {
      virtual ~Item() {}
      QString name_;
    };

    struct Items : QList<Item*> {
    };

    struct ItemNode : Item {
      ItemNode(const QMetaObject* mobj, QString name = "") {
        Q_ASSERT(mobj != nullptr);
        if (name == "")
          name = mobj->className();
        name_ = name;
        mobj_ = mobj;
      }
      ~ItemNode() override {}

      const QMetaObject* mobj_;
    };

    struct ItemCategory : Item {
      ItemCategory(QString name) {
        name_ = name;
      }
      ~ItemCategory() override {}

      Items items_;
    };

    Items items_;
  };

public:
  Nodes nodes_;
  Connections connections_;

  virtual Factory* factory() = 0;
};
