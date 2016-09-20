#ifdef QT_GUI_LIB

#include "ggraphwidget.h"

#include <QMessageBox>
#include "base/gjson.h"
#include "ggraphscene.h"

// ----------------------------------------------------------------------------
// GGraphWidget
// ----------------------------------------------------------------------------
GGraphWidget::GGraphWidget(QWidget *parent) : QWidget(parent) {
  init();
}

GGraphWidget::~GGraphWidget() {
}

void setColor(QWidget* widget, QColor color) { // gilgil temp 2016.09.18
  QPalette pal(color);
  widget->setAutoFillBackground(true);
  widget->setPalette(pal);
}

void GGraphWidget::init() {
  resize(QSize(640, 480));

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
  factoryWidget_ = new QTreeWidget(this);
  propWidget_ = new GPropWidget(this);
  graphView_ = new QGraphicsView(this);
  graphView_->setRenderHint(QPainter::Antialiasing);
  graphView_->setAcceptDrops(true);
  graphView_->setScene(new Scene(this));
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

  //setColor(toolBar_, Qt::black); // gilgil temp 2016.09.18
  //setColor(midSplitter_, Qt::blue); // gilgil temp 2016.09.18
  //setColor(statusBar_, Qt::red); // gilgil temp 2016.09.18
  //setColor(factoryWidget_, Qt::green); // gilgil temp 2016.09.18
  //setColor(propWidget_, Qt::yellow); // gilgil temp 2016.09.18
  //setColor(graphView_, Qt::darkGray); // gilgil temp 2016.09.18

  toolBar_->addAction(actionStart_);
  toolBar_->addAction(actionStop_);
  toolBar_->addSeparator();
  toolBar_->addAction(actionEdit_);
  toolBar_->addAction(actionLink_);
  toolBar_->addSeparator();
  toolBar_->addAction(actionOption_);

  QObject::connect(actionStart_, &QAction::triggered, this, &GGraphWidget::actionStartTriggered);
  QObject::connect(actionStop_, &QAction::triggered, this, &GGraphWidget::actionStopTriggered);
  QObject::connect(actionEdit_, &QAction::triggered, this, &GGraphWidget::actionEditTriggered);
  QObject::connect(actionLink_, &QAction::triggered, this, &GGraphWidget::actionLinkTriggered);
  QObject::connect(actionDelete_, &QAction::triggered, this, &GGraphWidget::actionDeleteTriggered);
  QObject::connect(actionOption_, &QAction::triggered, this, &GGraphWidget::actionOptionTriggered);

  QObject::connect(factoryWidget_, &QTreeWidget::clicked, this, &GGraphWidget::factoryWidgetClicked);
      // ----- gilgil temp 2016.09.20 -----
  QObject::connect(factoryWidget_, &QTreeWidget::doubleClicked, this, &GGraphWidget::doubleClicked);
  // ----------------------------------
}

void GGraphWidget::setGraph(GGraph* graph) {
  if (graph == graph_) return;
  // clear(); // gilgil temp 2016.09.17

  // gilgil temp 2016.09.17

  graph_ = graph;
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
  GGraph::Node* node = (GGraph::Node*)GObj::createInstance(className);
  if (node == nullptr) {
    QString msg = "can not create object for \"" + className + "\"";
    QMessageBox::information(NULL, "error", msg);
    return nullptr;
  }
  // ----- gilgil temp 2016.09.20 -----
  static int count = 0;
  QString tempName = "mmm" + QString::number(++count);
  node->setObjectName(tempName); // gilgil temp 2016.09.21
  // ----------------------------------
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
}

void GGraphWidget::propSave(QJsonObject& jo) {
  jo["rect"] << GJson::rect(this);

  QJsonObject splitter;
  splitter["mid"] << GJson::splitterSizes(midSplitter_);
  splitter["left"] << GJson::splitterSizes(midLeftSplitter_);
  splitter["prop"] << GJson::headerSizes(propWidget_);
  jo["splitter"] = splitter;
}

void GGraphWidget::actionStartTriggered(bool checked) {
  (void)checked;
  qDebug() << ""; // gilgil temp 2016.09.18
}

void GGraphWidget::actionStopTriggered(bool checked) {
  (void)checked;
  qDebug() << ""; // gilgil temp 2016.09.18
}

void GGraphWidget::actionEditTriggered(bool checked) {
  (void)checked;
  qDebug() << ""; // gilgil temp 2016.09.18
}

void GGraphWidget::actionLinkTriggered(bool checked) {
  (void)checked;
  qDebug() << ""; // gilgil temp 2016.09.18
}

void GGraphWidget::actionDeleteTriggered(bool checked) {
  (void)checked;
  qDebug() << ""; // gilgil temp 2016.09.18
}

void GGraphWidget::actionOptionTriggered(bool checked) {
  (void)checked;
  qDebug() << ""; // gilgil temp 2016.09.18
}

void GGraphWidget::factoryWidgetClicked(const QModelIndex &index) {
  (void)index;
  if (factoryWidget_->selectedItems().isEmpty()) return;
  Scene* scene = dynamic_cast<Scene*>(graphView_->scene());
  Q_ASSERT(scene != nullptr);
  scene->setMode(Scene::InsertItem);
}

// ----- gilgil temp 2016.09.20 -----
void GGraphWidget::doubleClicked(const QModelIndex &index) {
  (void)index;
  createNodeIfItemNodeSelected();
}
// ----------------------------------

#endif // QT_GUI_LIB
