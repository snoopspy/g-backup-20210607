#include "gpropitem_filepath.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemFilePath
// ----------------------------------------------------------------------------
void GPropItemFilePath::init(QTreeWidget* treeWidget, QTreeWidgetItem* parentItem, QObject* object, QMetaProperty mpro) {
  GPropItemWidget::init(treeWidget, parentItem, object, mpro);
  layout_ = new QHBoxLayout;
  lineEdit_ = new QLineEdit(treeWidget);
  toolButton_ = new QToolButton(treeWidget);
  fd_ = new QFileDialog(treeWidget);
  layout_->setMargin(0);
  lineEdit_->setFrame(false);
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

void GPropItemFilePath::myToolButtonClicked(bool checked) {
  (void)checked;
  fd_->setFileMode(QFileDialog::AnyFile);
  if (fd_->exec()) {
    bool res = object_->setProperty(mpro_.name(), QVariant::fromValue<QString>(fd_->selectedFiles().at(0)));
    if (!res) {
      qWarning() << QString("object->setProperty(%1, %2) return false").arg(mpro_.name(), lineEdit_->text());
    }
    update();
  }
}

#endif // QT_GUI_LIB
