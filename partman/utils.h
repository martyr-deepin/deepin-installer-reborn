// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_PARTMAN_UTILS_H
#define INSTALLER_PARTMAN_UTILS_H

#include "partman/structs.h"

namespace installer {

// Get maximum device size in bytes, by parsing /proc/partitions.
qint64 GetMaximumDeviceSize();

// Returns partition table type of the first disk device.
PartitionTableType GetPrimaryDiskPartitionTable();

}  // namespace installer

#endif  // INSTALLER_PARTMAN_UTILS_H
