// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/os_prober.h"

#include "base/command.h"

namespace partman {

OsTypeItems GetOsTypeItems() {
  OsTypeItems result;

  QString output;
  if (base::SpawnCmd("os-prober", {}, output)) {
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
      result.insert(items[0], type);
    }
  }

  return result;
}

}  // namespace partman