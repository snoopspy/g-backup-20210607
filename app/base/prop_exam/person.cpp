#include "person.h"
#include <GPropItemFilePath>

#ifdef QT_GUI_LIB

GPropItem* Person::createPropItem(QTreeWidgetItem* parent, QObject* object, QMetaProperty mpro) {
  if (QString(mpro.name()) == "picture") {
    GPropItemFilePath* res = new GPropItemFilePath(GPropItemParam(parent->treeWidget(), parent, object, mpro));
    res->fd_->setNameFilter("Image files - *.png *.xpm *.jpg (*.png *.xpm *.jpg);;Any files - * (*)");
    return res;
  }
  return GObj::createPropItem(parent, object, mpro);
}

#endif // QT_GUI_LIB
