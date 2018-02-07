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

#include "sysinfo/virtual_machine.h"

#include "base/file_util.h"

namespace installer {

// TODO(xushaohua): Call dmidecode or virt-what.
bool IsVirtualMachine() {
  // Check "hypervisor" flag is added in /proc/cpuinfo.
  const QString content(ReadFile("/proc/cpuinfo"));
  return content.contains("hypervisor");
}

}  // namespace installer