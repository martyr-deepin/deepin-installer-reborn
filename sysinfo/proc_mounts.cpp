// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/proc_mounts.h"

#include "base/file_util.h"

namespace installer {

QList<MountItem> ParseMountItems() {
  MountItemList result;
  const QString content(ReadFile("/proc/mounts"));
  const QStringList lines = content.split("\n");
  for (const QString& line : lines) {
    const QStringList parts = line.split(" ", QString::SkipEmptyParts);
    if (parts.length() == 6) {
      const MountItem item = { parts[0], parts[1], parts[2], parts[3] };
      result.append(item);
    }
  }

  return result;
}

}  // namespace installer