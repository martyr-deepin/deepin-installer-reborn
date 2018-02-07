/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "sysinfo/proc_partitions.h"

#include <QDebug>

#include "base/file_util.h"

namespace installer {

QList<PartitionItem> ParsePartitionItems() {
  PartitionItemList result;

  const QString content(ReadFile("/proc/partitions"));
  const QStringList lines(content.split("\n", QString::SkipEmptyParts));
  const QRegExp space_exp("\\s+");
  for (const QString& line : lines) {
    // Skip header line.
    if (line.startsWith("major")) {
      continue;
    }
    const QStringList parts(line.split(space_exp, QString::SkipEmptyParts));
    if (parts.length() != 4) {
      qWarning() << "Invalid partition item:" << parts;
      continue;
    }
    const PartitionItem item = {parts[0].toInt(), parts[1].toInt(),
                                parts[2].toLongLong(), parts[3]};
    result.append(item);
  }

  return result;
}

}  // namespace installer