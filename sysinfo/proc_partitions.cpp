// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/proc_partitions.h"

#include <QDebug>
#include <QRegExp>
#include "base/file_util.h"

namespace sysinfo {

QList<PartitionItem> ParsePartitionItems() {
  QList<PartitionItem> result;

  const QString content =
      base::ReadTextFileContent(QStringLiteral("/proc/partitions"));
  const QStringList lines = content.split("\n", QString::SkipEmptyParts);
  const QRegExp space_exp("\\s+");
  for (const QString& line : lines) {
    // Skip header line.
    if (line.startsWith("major")) {
      continue;
    }
    const QStringList parts = line.split(space_exp, QString::SkipEmptyParts);
    if (parts.length() != 4) {
      qWarning() << "Invalid partition item:" << parts;
      continue;
    }
    const PartitionItem item = { parts[0].toInt(), parts[1].toInt(),
                                 parts[2].toLongLong(), parts[3] };
    result.append(item);
  }

  return result;
}

}  // namespace sysinfo