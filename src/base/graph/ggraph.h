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
struct G_EXPORT GGraph : GStateObj {
  Q_OBJECT

public:
  // --------------------------------------------------------------------------
  // GObj
  // --------------------------------------------------------------------------
  typedef GObj Node;

  // --------------------------------------------------------------------------
  // Nodes
  // --------------------------------------------------------------------------
  struct Nodes : public QList<Node*> {
    void clear();
    GGraph::Node* findNode(QString objectName);
    void load(GGraph* graph, QJsonArray ja);
    void save(QJsonArray& ja);
  };

  // --------------------------------------------------------------------------
  // Connection
  // --------------------------------------------------------------------------
  struct Connection {
    Node* sender_{nullptr};
    QString signal_{""};
    Node* receiver_{nullptr};
    QString slot_{""};
  };

  // --------------------------------------------------------------------------
  // Connections
  // --------------------------------------------------------------------------
  struct Connections : QList<Connection*> {
    void clear();
    void load(GGraph* graph, QJsonArray ja);
    void save(QJsonArray& ja);
  };

  // --------------------------------------------------------------------------
  // Factory
  // --------------------------------------------------------------------------
  struct Factory : GObj {
    // ------------------------------------------------------------------------
    // Item
    // ------------------------------------------------------------------------
    struct Item {
      Item(QString displayName) : displayName_(displayName) {}
      virtual ~Item() {}
      QString displayName_; // class name or category name
    };
    typedef Item *PItem;

    // ------------------------------------------------------------------------
    // Items
    // ------------------------------------------------------------------------
    struct Items : QList<Item*> {
    };

    // ------------------------------------------------------------------------
    // ItemNode
    // ------------------------------------------------------------------------
    struct ItemNode : Item {
      ItemNode(QString className) : Item(className), className_(className) {}
      ~ItemNode() override {}

      QString className_;
    };

    // ------------------------------------------------------------------------
    // ItemCategory
    // ------------------------------------------------------------------------
    struct ItemCategory : Item {
      ItemCategory(QString categoryName) : Item(categoryName), categoryName_(categoryName) {}
      ~ItemCategory() override {}

      QString categoryName_;
      Items items_;
    };
    // ------------------------------------------------------------------------

  public:
    Factory(QObject* parent = nullptr);
    ~Factory() override;

  public:
    Items items_;
  };
  // --------------------------------------------------------------------------

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
