// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "sysinfo/virtual_machine.h"

#include "base/file_util.h"

namespace sysinfo {

// TODO(xushaohua): Call dmidecode or virt-what.
bool IsVirtualMachine() {
  // Check "hypervisor" flag is added in /proc/cpuinfo.
  const QString content =
      base::ReadTextFileContent(QStringLiteral("/proc/cpuinfo"));
  return content.indexOf(QStringLiteral("hypervisor"));
}

}  // namespace sysinfo