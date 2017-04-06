// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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