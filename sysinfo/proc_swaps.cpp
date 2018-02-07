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

#include "sysinfo/proc_swaps.h"

#include <QRegExp>
#include <QStringList>

#include "base/file_util.h"

namespace installer {

SwapItemList ParseSwaps() {
  SwapItemList result;
  const QString content(ReadFile("/proc/swaps"));

  for (const QString& line : content.split('\n')) {
    if ((!line.isEmpty()) && (!line.startsWith("Filename"))) {
      const QStringList parts(line.split(QRegExp("\\s+")));
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

}  // namespace installer