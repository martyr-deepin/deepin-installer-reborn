// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SYSINFO_PROC_MEMINFO_H
#define INSTALLER_SYSINFO_PROC_MEMINFO_H

#include <QtCore/QtGlobal>

namespace installer {

// MemInfo refers content of /proc/meminfo file.
// All of these values are in bytes.
struct MemInfo {
  qint64 buffers = 0;
  qint64 cached = 0;
  qint64 mem_available = 0;
  qint64 mem_free = 0;
  qint64 mem_total = 0;
  qint64 swap_free = 0;
  qint64 swap_total = 0;
};

MemInfo GetMemInfo();

}  // namespace installer

#endif  // INSTALLER_SYSINFO_PROC_MEMINFO_H
