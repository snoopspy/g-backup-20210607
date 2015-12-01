#include "gpropitem_filepath.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemDirPath
// ----------------------------------------------------------------------------
GPropItemFilePath::GPropItemFilePath(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro)
  : GPropItemWidget(parent, object, mpro) {
  QTreeWidget* treeWidget = item_->treeWidget();
  Q_ASSERT(treeWidget != nullptr);
  layout_ = new QHBoxLayout;
  lineEdit_ = new QLineEdit(treeWidget);
  toolButton_ = new QToolButton(treeWidget);
  layout_->setMargin(0);
  // lineEdit_->setFrame(false); // gilgil temp 2015.11.17
  toolButton_->setText("...");
  layout_->addWidget(lineEdit_);
  layout_->addWidget(toolButton_);
  widget_->setLayout(layout_);

  QObject::connect(lineEdit_, SIGNAL(editingFinished()), this, SLOT(myEditingFinished()));
  QObject::connect(toolButton_, SIGNAL(clicked(bool)), this, SLOT(myToolButtonClicked(bool)));
}

void GPropItemFilePath::update() {
  QVariant value = object_->property(mpro_.name());
  lineEdit_->setText(value.toString());
}

void GPropItemFilePath::myEditingFinished() {
  bool res = object_->setProperty(mpro_.name(), QVariant::fromValue<QString>(lineEdit_->text()));
  if (!res) {
    qWarning() << QString("object->setProperty(%1, %2) return false").arg(mpro_.name(), lineEdit_->text());
  }
  update();
}

#include <QFileDialog>
void GPropItemFilePath::myToolButtonClicked(bool checked) {
  (void)checked;
  QFileDialog fd;
  fd.setFileMode(QFileDialog::AnyFile);
  if (fd.exec()) {
    bool res = object_->setProperty(mpro_.name(), QVariant::fromValue<QString>(fd.selectedFiles().at(0)));
    if (!res) {
      qWarning() << QString("object->setProperty(%1, %2) return false").arg(mpro_.name(), lineEdit_->text());
    }
    update();
  }
}

#endif // QT_GUI_LIB
