#include "gpluginfactory.h"
#include <QDir>

// ----------------------------------------------------------------------------
// GPluginFactory
// ----------------------------------------------------------------------------
GPluginFactory::GPluginFactory(QObject* parent) : GGraph::Factory(parent) {
}

GPluginFactory::~GPluginFactory() {
  foreach (QLibrary* library, libraries_) {
    delete library;
  }
}

void GPluginFactory::load(QString folder) {
  if (!folder.endsWith("/"))
    folder += "/";
  loadFolder(nullptr, folder);
}

void GPluginFactory::loadFile(GGraph::Factory::ItemCategory* category, QString fileName) {
  QLibrary* library = new QLibrary(fileName);
  if (!library->load()) {
    qWarning() << QString("library->load return false for (%1) %2").arg(fileName, library->errorString());
    delete library;
    return;
  }

  typedef int (*CountFunc)();
  typedef char* (*NameFunc)(int index);
  typedef void* (*CreateFunc)(int index);

  CountFunc countFunc = (CountFunc)library->resolve("count");
  if (!countFunc) {
    qWarning() << QString("can not file 'count' function for (%1)").arg(fileName);
    delete library;
    return;
  }
  NameFunc nameFunc = (NameFunc)library->resolve("name");
  if (!nameFunc) {
    qWarning() << QString("can not file 'name' function for (%1)").arg(fileName);
    delete library;
    return;
  }
  CreateFunc createFunc = (CreateFunc)library->resolve("create");
  if (!createFunc) {
    qWarning() << QString("can not file 'create' function for (%1)").arg(fileName);
    delete library;
    return;
  }

  int count = countFunc();
  for (int i = 0; i < count; i++) {
    GGraph::Node* node = dynamic_cast<GGraph::Node*>((GGraph::Node*)createFunc(i));
    if (category == nullptr)
      this->items_.push_back(new ItemNode(node->metaObject()));
    else
      category->items_.push_back(new ItemNode(node->metaObject()));
    delete node;
  }

  this->libraries_.push_back(library);
}

void GPluginFactory::loadFolder(GGraph::Factory::ItemCategory* category, QString folder) {
  QDir dir(folder);

  //
  // file
  //
  QFileInfoList fileList = dir.entryInfoList(QStringList("*.so"));
  foreach (QFileInfo fileInfo, fileList) {
    QString fileName = fileInfo.filePath();
    loadFile(category, fileName);
  }

  //
  // dir
  //
  QFileInfoList dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
  foreach (QFileInfo fileInfo, dirList) {
    QString fileName = fileInfo.fileName();
    QString subFolder = folder + fileName + "/";
    ItemCategory* subCategory = new ItemCategory(fileName);
    if (category == nullptr)
      this->items_.push_back(subCategory);
    else
      category->items_.push_back(subCategory);
    loadFolder(subCategory, subFolder);
  }
}
