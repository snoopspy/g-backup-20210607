#ifdef QT_GUI_LIB

#include "ggraphwidget.h"

#include <QMessageBox>
#include "base/gjson.h"

// ----------------------------------------------------------------------------
// GGraphWidget
// ----------------------------------------------------------------------------
GGraphWidget::GGraphWidget(QWidget *parent) : QWidget(parent) {
  init();
  setControl();
}

GGraphWidget::~GGraphWidget() {
  clear();
}

void setColor(QWidget* widget, QColor color) { // gilgil temp 2016.09.18
  QPalette pal(color);
  widget->setAutoFillBackground(true);
  widget->setPalette(pal);
}

void GGraphWidget::init() {
  resize(QSize(640, 480));

  QStringList filters; filters << "graph files(*.graph)" << "any files(*)";
  fileDialog_.setNameFilters(filters);
  fileDialog_.setDefaultSuffix("graph");
  fileDialog_.setViewMode(QFileDialog::Detail);

  (actionNewFile_ = new QAction(this))->setText("New");
  (actionOpenFile_ = new QAction(this))->setText("Open");
  (actionSaveFile_ = new QAction(this))->setText("Save");
  (actionSaveFileAs_ = new QAction(this))->setText("Save As");
  (actionStart_ = new QAction(this))->setText("Start");
  (actionStop_ = new QAction(this))->setText("Stop");
  (actionEdit_ = new QAction(this))->setText("Edit");
  (actionLink_ = new QAction(this))->setText("Link");
  (actionDelete_ = new QAction(this))->setText("Delete");
  (actionOption_ = new QAction(this))->setText("Option");

  mainLayout_ = new QVBoxLayout(this);
  mainLayout_->setMargin(0);
  mainLayout_->setSpacing(0);

  toolBar_ = new QToolBar(this);
  midSplitter_ = new QSplitter(Qt::Horizontal, this);
  midLeftSplitter_ = new QSplitter(Qt::Vertical, this);
  (factoryWidget_ = new QTreeWidget(this))->setHeaderLabel("node");
  propWidget_ = new GPropWidget(this);
  scene_ = new GGScene(this);
  graphView_ = new QGraphicsView(this);
  graphView_->setRenderHint(QPainter::Antialiasing);
  graphView_->setAcceptDrops(true);
  graphView_->setScene(scene_);
  statusBar_ = new QStatusBar(this);

  mainLayout_->addWidget(toolBar_ , 0);
  mainLayout_->addWidget(midSplitter_, 1);
  mainLayout_->addWidget(statusBar_, 0);
  this->setLayout(mainLayout_);

  midSplitter_->addWidget(midLeftSplitter_);
  midSplitter_->addWidget(graphView_);
  midSplitter_->setStretchFactor(0, 0);
  midSplitter_->setStretchFactor(1, 1);

  midLeftSplitter_->addWidget(factoryWidget_);
  midLeftSplitter_->addWidget(propWidget_);
  midLeftSplitter_->setStretchFactor(0, 0);
  midLeftSplitter_->setStretchFactor(1, 1);

  toolBar_->addAction(actionNewFile_);
  toolBar_->addAction(actionOpenFile_);
  toolBar_->addAction(actionSaveFile_);
  toolBar_->addAction(actionSaveFileAs_);
  toolBar_->addSeparator();
  toolBar_->addAction(actionStart_);
  toolBar_->addAction(actionStop_);
  toolBar_->addSeparator();
  toolBar_->addAction(actionEdit_);
  toolBar_->addAction(actionLink_);
  toolBar_->addSeparator();
  toolBar_->addAction(actionDelete_);
  toolBar_->addSeparator();
  toolBar_->addAction(actionOption_);

  QObject::connect(actionNewFile_, &QAction::triggered, this, &GGraphWidget::actionNewFileTriggered);
  QObject::connect(actionOpenFile_, &QAction::triggered, this, &GGraphWidget::actionOpenFileTriggered);
  QObject::connect(actionSaveFile_, &QAction::triggered, this, &GGraphWidget::actionSaveFileTriggered);
  QObject::connect(actionSaveFileAs_, &QAction::triggered, this, &GGraphWidget::actionSaveFileAsTriggered);
  QObject::connect(actionStart_, &QAction::triggered, this, &GGraphWidget::actionStartTriggered);
  QObject::connect(actionStop_, &QAction::triggered, this, &GGraphWidget::actionStopTriggered);
  QObject::connect(actionEdit_, &QAction::triggered, this, &GGraphWidget::actionEditTriggered);
  QObject::connect(actionLink_, &QAction::triggered, this, &GGraphWidget::actionLinkTriggered);
  QObject::connect(actionDelete_, &QAction::triggered, this, &GGraphWidget::actionDeleteTriggered);
  QObject::connect(actionOption_, &QAction::triggered, this, &GGraphWidget::actionOptionTriggered);

  QObject::connect(factoryWidget_, &QTreeWidget::clicked, this, &GGraphWidget::factoryWidgetClicked);
  QObject::connect(scene_, &GGScene::selectionChanged, this, &GGraphWidget::setControl);
}

void GGraphWidget::setGraph(GGraph* graph) {
  if (graph == graph_) return;
  graph_ = graph;
  update();
  setControl();
}

void GGraphWidget::update() {
  factoryWidget_->clear();
  Q_ASSERT(graph_ != nullptr);
  GGraph::Factory* factory = graph_->factory();
  Q_ASSERT(factory!= nullptr);
  foreach(GGraph::Factory::Item* item, factory->items_) {
    updateFactory(item, nullptr);
  }
  factoryWidget_->expandAll();
}

void GGraphWidget::clear() {
  fileName_ = "";
  scene_->clear();
}

void GGraphWidget::loadGraph(QJsonObject jo) {
  graph_->propLoad(jo);
  // gilgil temp 2016.09.22
}

void GGraphWidget::saveGraph(QJsonObject& jo) {
  QJsonArray nodeJa;
  QObjectList objectList = scene_->children();
  foreach (QObject* object, objectList) {
    GGText* text = dynamic_cast<GGText*>(object);
    if (text != nullptr) {
      QJsonObject childJo;
      GObj* obj = text->obj_;
      childJo >> *obj;
      childJo["left"] = 999;
      nodeJa.append(childJo);
    }
  }
  jo["nodes"] = nodeJa;

  /*
  graph_->propSave(jo);
  QJsonArray ja = jo["nodes"].toArray();
  foreach (QJsonValue jv, ja) {
    QJsonObject& childJo = jv.toObject();
    qDebug() << childJo;
    childJo["left"] = 999;
    ja.append(childJo);
  }
  jo["nodes"] = ja;
  */
  // ----- gilgil temp 2016.09.22 -----
  /*
  foreach (GGraph::Node* node, graph_->nodes_) {
    QString objectName = node->objectName();
    GGText* text = scene_->findNodeByObjectName(objectName);
    if (gnode == nullptr) {
      qWarning() << QString("can not find (%1)").arg(objectName);
      continue;
    }
    gnode->boundingRect()
  }
  */
  // ----------------------------------

  // gilgil temp 2016.09.22
}

void GGraphWidget::updateFactory(GGraph::Factory::Item* item, QTreeWidgetItem* parent) {
  QTreeWidgetItem* newWidgetItem;
  if (parent != nullptr)
    newWidgetItem = new QTreeWidgetItem(parent);
  else
    newWidgetItem = new QTreeWidgetItem(factoryWidget_);

  newWidgetItem->setText(0, item->name_);
  QVariant v = QVariant::fromValue((void*)item);
  newWidgetItem->setData(0, Qt::UserRole, v);

  GGraph::Factory::ItemCategory* category = dynamic_cast<GGraph::Factory::ItemCategory*>(item);
  if (category != nullptr) {
    foreach (GGraph::Factory::Item* child, category->items_) {
      updateFactory(child, newWidgetItem);
    }
    return;
  }
}

GGraph::Node* GGraphWidget::createInstance(QString className) {
  GGraph::Node* node = graph()->createInstance(className);
  if (node == nullptr) return nullptr;

  QString objectName = node->metaObject()->className();
  foreach (QString removePrefixName, removePrefixNames_) {
    if (objectName.startsWith(removePrefixName))
      objectName = objectName.mid(removePrefixName.length());
  }
  if (objectName == "") {
    qWarning() << QString("objectName is empty for (%1)").arg(className);
    delete node;
    return nullptr;
  }
  if (toLowerFirstCharacter_) {
    char first = objectName.at(0).toLatin1();
    if (first >= 'A' && first <= 'Z') {
      first = first + 'a' - 'A';
      objectName = QString(first) + objectName.mid(1);
    }
  }

  int suffix = 1;
  while (true) {
    QString _objectName = objectName + QString::number(suffix);
    bool isExist = false;
    foreach(GGraph::Node* node, graph()->nodes_) {
      if (node->objectName() == _objectName) {
        isExist = true;
        break;
      }
    }
    if (!isExist) break;
    suffix++;
  }
  objectName = objectName + QString::number(suffix);
  node->setObjectName(objectName);

  return node;
}

GGraph::Node* GGraphWidget::createNodeIfItemNodeSelected() {
  QList<QTreeWidgetItem*> widgetItems = factoryWidget_->selectedItems();
  if (widgetItems.count() == 0)
    return nullptr;
  QTreeWidgetItem* widgetItem = widgetItems.at(0);
  QVariant variant = widgetItem->data(0, Qt::UserRole);
  void* p = qvariant_cast<void*>(variant);
  GGraph::Factory::Item* item = dynamic_cast<GGraph::Factory::Item*>((GGraph::Factory::Item*)p);
  GGraph::Factory::ItemNode* itemNode = dynamic_cast<GGraph::Factory::ItemNode*>(item);
  if (itemNode == nullptr)
    return nullptr;
  QString className = itemNode->mobj_->className();
  GGraph::Node* node = createInstance(className);
  if (node == nullptr) {
    QString msg = QString("createInstance failed for (%1)").arg(className);
    QMessageBox::warning(nullptr, "Error", msg);
    return nullptr;
  }
  node->setParent(graph_);
  graph_->nodes_.push_back(node);
  return node;
}

void GGraphWidget::propLoad(QJsonObject jo) {
  jo["rect"] >> GJson::rect(this);

  QJsonObject splitter = jo["splitter"].toObject();
  splitter["mid"] >> GJson::splitterSizes(midSplitter_);
  splitter["left"] >> GJson::splitterSizes(midLeftSplitter_);
  splitter["prop"] >> GJson::headerSizes(propWidget_);

  toLowerFirstCharacter_ = jo["toLowerFirstCharacter"].toBool();
  removePrefixNames_ = jo["removePrefixNames"].toString().split(",");
  ignoreSignalNames_ = jo["ignoreSignalNames"].toString().split(",");
  ignoreSlotNames_ = jo["ignoreSlotNames"].toString().split(",");

  loadGraph(jo["graph"].toObject());
}

void GGraphWidget::propSave(QJsonObject& jo) {
  jo["rect"] << GJson::rect(this);

  QJsonObject splitter;
  splitter["mid"] << GJson::splitterSizes(midSplitter_);
  splitter["left"] << GJson::splitterSizes(midLeftSplitter_);
  splitter["prop"] << GJson::headerSizes(propWidget_);
  jo["splitter"] = splitter;

  jo["toLowerFirstCharacter"] = toLowerFirstCharacter_;
  jo["removePrefixNames"] = removePrefixNames_.join(",");
  jo["ignoreSignalNames"] = ignoreSignalNames_.join(",");
  jo["ignoreSlotNames"] = ignoreSlotNames_.join(",");

  QJsonObject graphJo;
  saveGraph(graphJo);
  jo["graph"] = graphJo;
}

void GGraphWidget::setControl() {
  // ----- gilgil temp 2016.09.21 -----
  /*
  QString title = "SnoopSpy";
  if (fileName != "")
  {
    QFileInfo fi(fileName);
    title = fi.completeBaseName();
  }
  setWindowTitle(title);

  ui->actionSaveFile->setEnabled(m_changed && fileName != "");
  ui->actionSaveFileAs->setEnabled(m_changed);
  */
  // ----------------------------------

  GGScene::Mode mode = scene_->mode();

  actionEdit_->setEnabled(mode != GGScene::MoveItem);
  actionLink_->setEnabled(mode != GGScene::InsertLine);

  bool active = false;
  if (graph_ != nullptr)
    active = graph_->active();

  actionNewFile_->setEnabled(!active);
  actionOpenFile_->setEnabled(!active);
  actionSaveFile_->setEnabled(!active && fileName_ != "");
  actionSaveFileAs_->setEnabled(!active);
  actionStart_->setEnabled(!active);
  actionStop_->setEnabled(active);

  bool selected = scene_->selectedItems().count() > 0;

  actionDelete_->setEnabled(selected);
  // actionBringToFront_->setEnabled(selected); // gilgil temp 2016.09.21
  // actionSendToBack_->setEnabled(selected); // gilgil temp 2016.09.21

  GObj* selectedObj = nullptr;
  if (selected)
  {
    QGraphicsItem* item = scene_->selectedItems().first();
    GGText* text = dynamic_cast<GGText*>(item);
    if (text != nullptr)
      selectedObj = dynamic_cast<GObj*>(text->obj_);
  }
  propWidget_->setObject(selectedObj);
  actionOption_->setEnabled(selectedObj != nullptr);
}

void GGraphWidget::actionNewFileTriggered(bool) {
  clear();
  setControl();
}

void GGraphWidget::actionOpenFileTriggered(bool) {
  fileDialog_.setAcceptMode(QFileDialog::AcceptOpen);
  fileDialog_.setFileMode(QFileDialog::ExistingFile);
  if (fileDialog_.exec() == QDialog::Accepted) {
    fileName_ = fileDialog_.selectedFiles().first();
    QJsonObject jo = GJson::loadFromFile(fileName_);
    saveGraph(jo);
    setControl();
  }
}

void GGraphWidget::actionSaveFileTriggered(bool) {
  QJsonObject jo;
  saveGraph(jo);
  GJson::saveToFile(jo, fileName_);
  setControl();
}

void GGraphWidget::actionSaveFileAsTriggered(bool) {
  fileDialog_.setAcceptMode(QFileDialog::AcceptSave);
  fileDialog_.setFileMode(QFileDialog::AnyFile);
  if (fileDialog_.exec() == QDialog::Accepted)
  {
    fileName_ = fileDialog_.selectedFiles().first();
    actionSaveFileTriggered(false);
  }
}

void GGraphWidget::actionStartTriggered(bool) {
  bool res = graph_->open();
  if (!res) {
    QString msg = graph_->err->msg();
    QMessageBox::warning(nullptr, "Error", msg);
  }
  setControl();
}

void GGraphWidget::actionStopTriggered(bool) {
  qDebug() << ""; // gilgil temp 2016.09.18
  setControl();
}

void GGraphWidget::actionEditTriggered(bool) {
  qDebug() << ""; // gilgil temp 2016.09.18
  scene_->setMode(GGScene::MoveItem);
  setControl();
}

void GGraphWidget::actionLinkTriggered(bool) {
  qDebug() << ""; // gilgil temp 2016.09.18
  scene_->setMode(GGScene::InsertLine);
  setControl();
}

void GGraphWidget::actionDeleteTriggered(bool) {
  if (scene_->selectedItems().count() == 0)
    return;
  QGraphicsItem* item = scene_->selectedItems().first();
  GGText* text = dynamic_cast<GGText*>(item);
  if (text != nullptr)
    delete text;
  setControl();
}

void GGraphWidget::actionOptionTriggered(bool) {
  qDebug() << ""; // gilgil temp 2016.09.18
}

void GGraphWidget::factoryWidgetClicked(const QModelIndex&) {
  if (factoryWidget_->selectedItems().isEmpty()) return;
   scene_->setMode(GGScene::InsertItem);
}

#endif // QT_GUI_LIB
