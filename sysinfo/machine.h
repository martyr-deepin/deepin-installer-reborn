// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SYSINFO_MACHINE_H
#define DEEPIN_INSTALLER_REBORN_SYSINFO_MACHINE_H

namespace sysinfo {

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

}  // namespace sysinfo

#endif  // DEEPIN_INSTALLER_REBORN_SYSINFO_MACHINE_H
