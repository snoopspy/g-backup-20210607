#include "person.h"
#include <GPropItemFilePath>

#ifdef QT_GUI_LIB

GPropItem* Person::propCreateItem(GPropItemParam* param) {
  if (QString(param->mpro_.name()) == "picture") {
    GPropItemFilePath* res = new GPropItemFilePath(param);
    QStringList filters{"image files - *.png *.xpm *.jpg(*.png *.xpm *.jpg)", "any files - *(*)"};
    res->fd_->setNameFilters(filters);
    return res;
  }
  return GObj::propCreateItem(param);
}

#endif // QT_GUI_LIB
