// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/dev_disk.h"

#include <QDir>

namespace sysinfo {

namespace {

QHash<QString, QString> ParseDevDir(const QString& dir_name) {
  QHash<QString, QString> result;

  QDir dir(dir_name);
  const QFileInfoList items =
      dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
  for (const QFileInfo& info : items) {
    result.insert(info.canonicalFilePath(), info.fileName());
  }

  return result;
};

}  // namespace

QHash<QString, QString> ParseLabelDir() {
  return ParseDevDir("/dev/disk/by-label");
}

QHash<QString, QString> ParsePartLabelDir() {
  return ParseDevDir("/dev/disk/by-partlabel");
};

QHash<QString, QString> ParseUUIDDir() {
  return ParseDevDir("/dev/disk/by-uuid");
}

}  // namespace sysinfo