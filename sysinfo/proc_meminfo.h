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
