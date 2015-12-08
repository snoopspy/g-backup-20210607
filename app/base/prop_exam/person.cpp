#include "person.h"
#include <GPropItemFilePath>

#ifdef QT_GUI_LIB

GPropItem* Person::createPropItem(GPropItemParam param) {
  if (QString(param.mpro_.name()) == "picture") {
    GPropItemFilePath* res = new GPropItemFilePath(param);
    res->fd_->setNameFilter("Image files - *.png *.xpm *.jpg (*.png *.xpm *.jpg);;Any files - * (*)");
    return res;
  }
  return GObj::createPropItem(param);
}

#endif // QT_GUI_LIB
