// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/os_prober.h"

#include "base/command.h"
#include "base/file_util.h"

namespace installer {

namespace {

// Cache output of `os-prober` command.
QString ReadOsProberOutput() {
  const QString cache_path("/tmp/deepin-installer-os-prober.conf");
  if (QFile::exists(cache_path)) {
    return ReadFile(cache_path);
  } else {
    QString output;
    if (SpawnCmd("os-prober", {}, output)) {
      WriteTextFile(cache_path, output);
      return output;
    } else {
      qCritical() << "os-prober failed";
      return QString();
    }
  }
}

}  // namespace

OsTypeItems GetOsTypeItems() {
  OsTypeItems result;

  const QString output = ReadOsProberOutput();
  if (!output.isEmpty()) {
    for (const QString& line : output.split('\n')) {
      if (line.isEmpty()) {
        continue;
      }

      const QStringList items = line.split(':');
      Q_ASSERT(items.length() == 4);
      if (items.length() != 4) {
        continue;
      }

      OsType type;
      if (line.contains("linux", Qt::CaseInsensitive)) {
        type = OsType::Linux;
      } else if (line.contains("windows", Qt::CaseInsensitive)) {
        type = OsType::Windows;
      } else if (line.contains("mac", Qt::CaseInsensitive)) {
        // FIXME(xushaohua): Check macOS efi entry.
        type = OsType::Mac;
      } else {
        type = OsType::Unknown;
      }

      // Deal with EFI path, like "/dev/sda2@/EFI/Microsoft/Boot/bootmgfw.efi.
      const QString dev_path = items[0];
      const int index = dev_path.indexOf('@');
      const QString path = (index == -1) ? dev_path : dev_path.left(index);
      result.insert(path, type);
    }
  }

  return result;
}

}  // namespace installer