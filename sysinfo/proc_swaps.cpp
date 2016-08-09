// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/proc_swaps.h"

#include <QRegExp>
#include <QStringList>

#include "base/file_util.h"

namespace sysinfo {

SwapItemList ParseSwaps() {
  SwapItemList result;
  const QString content =
      base::ReadTextFileContent(QStringLiteral("/proc/swaps"));

  for (const QString& line : content.split('\n')) {
    if ((!line.isEmpty()) && (!line.startsWith("Filename"))) {
      const QStringList parts = line.split(QRegExp("\\s+"));
      if (parts.length() == 5) {
        SwapItem item = {
            parts.at(0),
            parts.at(1) == "partition" ? SwapType::Partition : SwapType::File,
            parts.at(2).toLongLong() * 1000,  // from kilobytes
            parts.at(3).toLongLong() * 1000,  // from kilobytes
            parts.at(4).toInt(),
        };
        result.append(item);
      }
    }
  }

  return result;
}

}  // namespace sysinfo