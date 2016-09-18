// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_UTILS_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_UTILS_H

#include "partman/structs.h"

namespace partman {

// Check if EFI feature is enabled in this machine.
bool IsEfiEnabled();

// Get maximum device size, by parsing /proc/partitions.
qint64 GetMaximumDeviceSize();

// Returns partition table type of the first disk device.
PartitionTableType GetPrimaryDiskPartitionTable();

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_UTILS_H
