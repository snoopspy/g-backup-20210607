#include "gpropitemlineeditqchar.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemLineEditQChar
// ----------------------------------------------------------------------------
GPropItemLineEditQChar::GPropItemLineEditQChar(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro)
  : GPropItemLineEdit(parent, object, mpro) {
  QObject::connect(lineEdit_, SIGNAL(editingFinished()), this, SLOT(myEditingFinished()));
}

void GPropItemLineEditQChar::update() {
  QVariant value = object_->property(mpro_.name());
  lineEdit_->setText(value.toString());
}

void GPropItemLineEditQChar::myEditingFinished() {
  QChar value;
  QString text = lineEdit_->text();
  if (text.length() == 0) value = '\0';
  else value = text.at(0);
  bool res = object_->setProperty(mpro_.name(), value);
  if (!res) {
    qWarning() << QString("object->setProperty(%1, %2) return false").arg(mpro_.name(), lineEdit_->text());
  }
  update();
}

#endif // QT_GUI_LIB
