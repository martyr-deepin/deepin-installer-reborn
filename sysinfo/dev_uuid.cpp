// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/dev_uuid.h"

#include <QDir>
#include <QFileInfoList>

namespace sysinfo {

QHash<QString, QString> ParseUUIDDir() {
  QHash<QString, QString> result;

  QDir uuid_dir("/dev/disk/by-uuid");
  const QFileInfoList items =
      uuid_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
  for (const QFileInfo& info : items) {
    result.insert(info.canonicalFilePath(), info.fileName());
  }

  return result;
}

}  // namespace sysinfo