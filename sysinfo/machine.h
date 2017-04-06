// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SYSINFO_MACHINE_H
#define INSTALLER_SYSINFO_MACHINE_H

#include <QString>

namespace installer {

// Does not distinguish 32bit and 64bit architecture.
enum class MachineArch {
  ARM,
  LOONGSON,
  SW,
  X86,
  Unknown,
};

// Returns current machine architecture.
MachineArch GetMachineArch();

// Returns name of current machine architecture.
QString GetMachineArchName();

}  // namespace installer

#endif  // INSTALLER_SYSINFO_MACHINE_H
