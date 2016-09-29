// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_SYSINFO_MACHINE_H
#define INSTALLER_SYSINFO_MACHINE_H

namespace installer {

enum class MachineArch {
  Alpha,
  Alpha64,
  ARM,
  ARM64,
  MIPS,
  MIPS64,
  X86,
  X86_64,

  Unknown,
};

// Returns current machine architecture.
MachineArch GetMachineArch();

}  // namespace installer

#endif  // INSTALLER_SYSINFO_MACHINE_H
