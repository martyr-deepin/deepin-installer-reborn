// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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