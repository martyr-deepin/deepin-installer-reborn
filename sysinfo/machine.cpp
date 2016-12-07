// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/machine.h"

#include <sys/utsname.h>
#include <QString>

namespace installer {

namespace {

const char kUnknownArch[] = "unknown";

}  // namespace

MachineArch GetMachineArch() {
  struct utsname uts;

  if (uname(&uts) != 0) {
    return MachineArch::Unknown;
  }

  const QString machine(uts.machine);
  if (machine == "x86") {
    return MachineArch::X86;
  }
  if (machine == "x86_64") {
    return MachineArch::X86_64;
  }
  if (machine == "mips") {
    return MachineArch::MIPS;
  }
  if (machine == "mips64") {
    return MachineArch::MIPS64;
  }
  if (machine == "alpha") {
    return MachineArch::Alpha;
  }
  if (machine == "alpha64") {
    return MachineArch::Alpha64;
  }
  if (machine == "arm") {
    return MachineArch::ARM;
  }
  if (machine == "arm64") {
    return MachineArch::ARM64;
  }

  // TODO(xushaohua): Check other architectures.

  return MachineArch::Unknown;
}

QString GetMachineArchName() {
  struct utsname uts;

  if (uname(&uts) != 0) {
    return kUnknownArch;
  }

  return uts.machine;
}

}  // namespace installer