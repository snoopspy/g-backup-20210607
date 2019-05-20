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
  typedef char* (*MetaFunc)(int index);
  typedef void* (*CreateFunc)(int index);

  CountFunc countFunc = CountFunc(library->resolve("count"));
  if (countFunc == nullptr) {
    qWarning() << QString("can not file 'count' function for (%1)").arg(fileName);
    delete library;
    return;
  }
  MetaFunc metaFunc = MetaFunc(library->resolve("meta"));
  if (metaFunc == nullptr) {
    qWarning() << QString("can not file 'name' function for (%1)").arg(fileName);
    delete library;
    return;
  }
  CreateFunc createFunc =CreateFunc(library->resolve("create"));
  if (createFunc == nullptr) {
    qWarning() << QString("can not file 'create' function for (%1)").arg(fileName);
    delete library;
    return;
  }

  int count = countFunc();
  for (int i = 0; i < count; i++) {
    QMetaObject* mobj = (QMetaObject*)metaFunc(i);
    if (mobj == nullptr) {
      qCritical() << QString("call metaFunc(%1) return nullptr file=%2").arg(i).arg(fileName);
      break;
    }
    if (category == nullptr)
      this->items_.push_back(new ItemNode(mobj));
    else
      category->items_.push_back(new ItemNode(mobj));
  }

  this->libraries_.push_back(library);
}

void GPluginFactory::loadFolder(GGraph::Factory::ItemCategory* category, QString folder) {
  QDir dir(folder);

  //
  // file
  //
#ifdef Q_OS_LINUX
  QStringList files = QStringList("*.so");
#endif
#ifdef Q_OS_WIN
  QStringList files = QStringList("*.dll");
#endif
  QFileInfoList fileList = dir.entryInfoList(files);
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

GPluginFactory& GPluginFactory::instance() {
  static GPluginFactory pluginFactory;
  return pluginFactory;
}
