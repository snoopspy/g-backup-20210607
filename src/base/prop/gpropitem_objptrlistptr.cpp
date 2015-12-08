#include "base/gobj.h"
#include "gpropitem_objptrlistptr.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemObjPtrListMgr
// ----------------------------------------------------------------------------
struct GPropItemObjPtrListMgr : QObject, QTreeWidgetItem {
  Q_OBJECT

public:
  GPropItemObjPtrListMgr(QTreeWidgetItem* parent, GObjPtrListPtr list, GObj* obj)
    : QObject(parent->treeWidget()), QTreeWidgetItem(parent), list_(list),  obj_(obj) {
    int index = list->indexOf(obj);
    Q_ASSERT(index != -1);
    QString text = QString("item%1").arg(index);
    this->setText(0, text);
    this->setExpanded(true);

    QTreeWidget* treeWidget = parent->treeWidget();
    Q_ASSERT(treeWidget != nullptr);
    QToolButton* toolButton = new QToolButton(treeWidget);
    toolButton->setText("-");
    toolButton->setAutoRaise(true);
    treeWidget->setItemWidget(this, 1, toolButton);
    QObject::connect(toolButton, SIGNAL(clicked(bool)), this, SLOT(myClicked(bool)));
  }

  GObjPtrListPtr list_;
  GObj* obj_;

protected slots:
  void myClicked(bool checked) {
    (void)checked;
    list_->removeOne(obj_);
    delete this;
  }
};

// ----------------------------------------------------------------------------
// GPropItemObjPtrListPtr
// ----------------------------------------------------------------------------
GPropItemObjPtrListPtr::GPropItemObjPtrListPtr(GPropItemParam param) : GPropItemToolButton(param) {
  toolButton_->setText("+");
  QObject::connect(toolButton_, SIGNAL(clicked(bool)), this, SLOT(myClicked(bool)));
}

void GPropItemObjPtrListPtr::update() {
  while (item_->childCount() > 0) item_->removeChild(item_->child(0));
  const char* propName = mpro_.name();
  QVariant variant = object_->property(propName);
  GObjPtrListPtr list = qvariant_cast<GObjPtrListPtr>(variant);
  for (_GObjPtrList::iterator it = list->begin(); it != list->end(); it++) {
    GObj* childObj = *it;
    GPropItemObjPtrListMgr* mgr = new GPropItemObjPtrListMgr(item_, list, childObj);
    childObj->createPropItems(item_->treeWidget(), mgr, childObj);
  }
}

void GPropItemObjPtrListPtr::myClicked(bool checked) {
  (void)checked;
  const char* propName = mpro_.name();
  QVariant variant = object_->property(propName);
  GObjPtrListPtr list = qvariant_cast<GObjPtrListPtr>(variant);
  Q_ASSERT(list != nullptr);
  GObj* childObj = list->addObj();
  Q_ASSERT(childObj != nullptr);
  GPropItemObjPtrListMgr* mgr = new GPropItemObjPtrListMgr(item_, list, childObj);
  childObj->createPropItems(item_->treeWidget(), mgr, childObj);
}

#include "gpropitem_objptrlistptr.moc"

#endif // QT_GUI_LIB
