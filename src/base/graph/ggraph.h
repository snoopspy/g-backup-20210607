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

#ifdef QT_GUI_LIB

#include "base/gstateobj.h"
#include "base/gjson.h"

// ----------------------------------------------------------------------------
// GGraph
// ----------------------------------------------------------------------------
struct GGraph : GStateObj {
  typedef GObj Node;

  struct Nodes : public QList<Node*> {
    void clear();
    GGraph::Node* findNode(QString objectName);
    void load(QJsonArray ja);
    void save(QJsonArray& ja);
  };

  struct Connection {
    Node* sender_{nullptr};
    QString signal_{""};
    Node* receiver_{nullptr};
    QString slot_{""};
    // bool operator ==(const Connection& other); // gilgil temp 2016.09.23
  };

  struct Connections : QList<Connection*> {
    void clear();
    void load(GGraph* graph, QJsonArray ja);
    void save(QJsonArray& ja);
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
    Items items_;
  };

public:
  virtual Factory* factory() = 0;

protected:
  bool doOpen() override;
  bool doClose() override;

public:
  static Node* createInstance(QString className);

public:
  Nodes nodes_;
  Connections connections_;

public slots:
  void clear();
  void start();
  void stop();

public:
  void propLoad(QJsonObject jo) override;
  void propSave(QJsonObject& jo) override;
};

#endif // QT_GUI_LIB
