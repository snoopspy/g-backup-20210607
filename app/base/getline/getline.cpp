#include <QDir>

// ----------------------------------------------------------------------------
// GetLine
// ---------------------------------------------------------------------------
namespace GetLine {
  int getLineOfFileName(QString fileName) {
    static const int MAX_STR_SIZE = 65536;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) return 0;

    int res = 0;
    while (true) {
      char buf[MAX_STR_SIZE];
      qint64 readLen = file.readLine(buf, MAX_STR_SIZE);
      if (readLen == -1) break;
      res++;
    }
    file.close();
    printf("%s\t%d\n", QFileInfo(fileName).fileName().toLatin1().data(), res);
    return res;
  }

  int scan(QString folder, QStringList& nameFilters) {
    int res = 0;
    QDir dir(folder);
    bool isFolderWritten = false;
    QFileInfoList fileInfoList;

    //
    // file
    //
    fileInfoList = dir.entryInfoList(nameFilters);
    foreach (QFileInfo fileInfo, fileInfoList) {
      if (fileInfo.isDir()) continue;
      if (!isFolderWritten) {
        printf("--------------------------------------------------\n");
        printf("%s\n\n", folder.toLatin1().data());
        isFolderWritten = true;
      }
      res += getLineOfFileName(fileInfo.absoluteFilePath());
    }

    //
    // dir
    //
    fileInfoList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (QFileInfo fileInfo, fileInfoList) {
      QString fileName = fileInfo.fileName();
      QString subFolder = folder + fileName + "/";
      res += scan(subFolder, nameFilters);
    }
    return res;
  }
}

// ----------------------------------------------------------------------------
// Param
// ----------------------------------------------------------------------------
struct Param {
public:
  QString folder_;
  QStringList nameFilters_;

public:
  Param() {
    folder_ = "./";
    nameFilters_.clear();
  }

public:
  bool parse(int argc, char* argv[]) {
		int index = 1;
    while (index < argc) {
      if (strcmp(argv[index], "-f") == 0) {
        index++;
        if (index >= argc) {
          printf("invalid option\n");
          return false;
        }
        folder_ = argv[index];
        if (folder_[folder_.length() - 1] != '\\' && folder_[folder_.length() - 1] != '/')
          folder_ += '/';
      } else {
        QString nameFilter = argv[index];
        nameFilters_.push_back(nameFilter);
      }
      index++;
		}
    return nameFilters_.size() > 0;
	}

  static void usage() {
    printf("getline version 9.0\n");
    printf("Copyright (c) Gilbert Lee All rights reserved\n");
    printf("\n");
    printf("getline [-f <root folder>] <list of files>\n");
    printf("\n");
    printf("example\n");
    printf("\n");
    printf("  getline '*.h' '*.cpp'\n");
    printf("  getline '*.h*' '*.c*'\n");
    printf("  getline -f /usr/include '*.h'\n");
  }
};

// ----------------------------------------------------------------------------
// main
// ----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
	Param param;
  if (!param.parse(argc, argv)) {
    Param::usage();
		return 0;
	}
  int res = GetLine::scan(param.folder_, param.nameFilters_);
	printf("\n");
	printf("==================================================\n");
	printf("Total File Line = %d\n", res);
	printf("==================================================\n");

  return 0;
}

