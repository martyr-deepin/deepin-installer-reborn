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

#include "sysinfo/proc_meminfo.h"

#include <QDebug>

#include "base/file_util.h"

namespace installer {

namespace {

const char kMemInfoFile[] = "/proc/meminfo";

}  // namespace

MemInfo GetMemInfo() {
  const QString content = ReadFile(kMemInfoFile);
  if (content.isEmpty()) {
    qWarning() << "Failed to read meminfo!";
    return MemInfo();
  }

  QHash<QString, qint64> hash;
  for (const QString& line : content.split('\n')) {
    const int index = line.indexOf(':');
    if (index > -1) {
      QString str_value = line.mid(index + 1);
      str_value.remove("kB");
      str_value = str_value.trimmed();
      // Convert kB to byte.
      const qint64 value = str_value.toLongLong() * 1024;
      hash.insert(line.left(index), value);
    }
  }

  MemInfo info;
  info.buffers = hash.value("Buffers");
  info.cached = hash.value("Cached");
  info.mem_available = hash.value("MemAvailable");
  info.mem_free = hash.value("MemFree");
  info.mem_total = hash.value("MemTotal");
  info.swap_free = hash.value("SwapFree");
  info.swap_total = hash.value("SwapTotal");

  return info;
}

}  // namespace installer
