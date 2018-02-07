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

#include "sysinfo/machine.h"

#include <sys/utsname.h>
#include <QString>

namespace installer {

MachineArch GetMachineArch() {
  struct utsname uts;

  if (uname(&uts) != 0) {
    return MachineArch::Unknown;
  }

  const QString machine(uts.machine);
  if (machine.startsWith("arm", Qt::CaseInsensitive)) {
    return MachineArch::ARM;
  }
  if (machine.startsWith("mips", Qt::CaseInsensitive)) {
    // Remap MIPS as Loongson.
    return MachineArch::LOONGSON;
  }
  if (machine.startsWith("sw", Qt::CaseInsensitive)) {
    return MachineArch::SW;
  }
  if (machine.startsWith("x86", Qt::CaseInsensitive)) {
    return MachineArch::X86;
  }
  return MachineArch::Unknown;
}

QString GetMachineArchName() {
  switch (GetMachineArch()) {
    case MachineArch::ARM: {
      return "arm";
    }
    case MachineArch::LOONGSON: {
      return "loongson";
    }
    case MachineArch::SW: {
      return "sw";
    }
    case MachineArch::X86: {
      return "x86";
    }
    default: {
      return "unknown";
    }
  }
}

}  // namespace installer