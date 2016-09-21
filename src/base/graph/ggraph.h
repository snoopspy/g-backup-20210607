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
#include "base/gjson.h"

// ----------------------------------------------------------------------------
// GGraph
// ----------------------------------------------------------------------------
struct GGraph : GObj {
  typedef GObj Node;

  struct Nodes : QList<Node*>, GProp {
    void propLoad(QJsonObject jo) override;
    void propSave(QJsonObject& jo) override;
  };

  struct Connection {
    QString sender_;
    QString signal_;
    QString receiver_;
    QString slot_;
    bool operator ==(const Connection& other);
  };

  struct Connections : QList<Connection>, GProp {
    void propLoad(QJsonObject jo) override;
    void propSave(QJsonObject& jo) override;
  };

  struct Factory : GObj {
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

  public:
    Factory(GGraph* graph) : graph_(graph) {}

  protected:
    GGraph* graph_;

  public:
    Items items_;

  public:
    Node* createInstance(QString className);

  public:
    bool toLowerFirstCharacter_{true};
    QStringList removePrefixNames_{"G"};
    QStringList ignoreSignalNames_;
    QStringList ignoreSlotNames_;

    void propLoad(QJsonObject jo) override;
    void propSave(QJsonObject& jo) override;
  };

public:
  virtual Factory* factory() = 0;

public:
  Nodes nodes_;
  Connections connections_;

public:
  void clear();

public:
  void propLoad(QJsonObject jo) override;
  void propSave(QJsonObject& jo) override;
};
