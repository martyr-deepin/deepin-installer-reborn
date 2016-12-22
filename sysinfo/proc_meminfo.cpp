// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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