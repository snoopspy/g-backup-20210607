#ifdef QT_GUI_LIB

#include "ggraphwidget.h"

#include "base/gjson.h"

// ----------------------------------------------------------------------------
// GGraphWidget
// ----------------------------------------------------------------------------
GGraphWidget::GGraphWidget(QWidget *parent) : QWidget(parent) {
  init();
}

GGraphWidget::GGraphWidget(GGraph* graph) {
  init();
  setGraph(graph);
}

GGraphWidget::~GGraphWidget() {
}

void setColor(QWidget* widget, QColor color) { // gilgil temp 2016.09.18
  QPalette pal(color);
  widget->setAutoFillBackground(true);
  widget->setPalette(pal);
}

void GGraphWidget::init() {
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
  nodeFactoryWidget_ = new QTreeWidget(this);
  propWidget_ = new GPropWidget(this);
  midRightWidget_ = new QWidget(this);
  statusBar_ = new QStatusBar(this);
  statusBar_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

  mainLayout_->addWidget(toolBar_);
  mainLayout_->addWidget(midSplitter_);
  mainLayout_->addWidget(statusBar_);
  this->setLayout(mainLayout_);

  midSplitter_->addWidget(midLeftSplitter_);
  midSplitter_->addWidget(midRightWidget_);

  midLeftSplitter_->addWidget(nodeFactoryWidget_);
  midLeftSplitter_->addWidget(propWidget_);

  setColor(toolBar_, Qt::black); // gilgil temp 2016.09.18
  setColor(midSplitter_, Qt::blue); // gilgil temp 2016.09.18
  setColor(statusBar_, Qt::red); // gilgil temp 2016.09.18
  setColor(nodeFactoryWidget_, Qt::green); // gilgil temp 2016.09.18
  setColor(propWidget_, Qt::yellow); // gilgil temp 2016.09.18

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
}

void GGraphWidget::setGraph(GGraph* graph) {
  if (graph == graph_) return;
  // clear(); // gilgil temp 2016.09.17

  // gilgil temp 2016.09.17

  graph_ = graph;
}

void GGraphWidget::update() {

}

void GGraphWidget::propLoad(QJsonObject jo) {
  jo["rect"] >> GJson::rect(this);

  QJsonObject splitter = jo["splitter"].toObject();
  splitter["mid"] >> GJson::splitterSizes(midSplitter_);
  splitter["midLeft"] >> GJson::splitterSizes(midLeftSplitter_);
  splitter["propWidget"] >> GJson::headerSizes(propWidget_);
}

void GGraphWidget::propSave(QJsonObject& jo) {
  jo["rect"] << GJson::rect(this);

  QJsonObject splitter;
  splitter["mid"] << GJson::splitterSizes(midSplitter_);
  splitter["midLeft"] << GJson::splitterSizes(midLeftSplitter_);
  splitter["propWidget"] << GJson::headerSizes(propWidget_);
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

#endif // QT_GUI_LIB
