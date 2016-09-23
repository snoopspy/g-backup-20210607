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
    Factory(QObject* parent = nullptr);
    ~Factory() override;

  public:
    Items items_;
  };

public:
  GGraph(QObject* parent = nullptr) : GStateObj(parent) {}
  ~GGraph() override {}

public:
  Factory* factory() { return factory_; }
  void setFactory(Factory* factory) { factory_ = factory; }

protected:
  Factory* factory_{nullptr};

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
