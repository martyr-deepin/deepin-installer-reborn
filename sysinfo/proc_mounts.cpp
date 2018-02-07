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