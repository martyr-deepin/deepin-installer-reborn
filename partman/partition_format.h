// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_FORMAT_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_FORMAT_H

#include <QString>

#include "partman/partition.h"

namespace partman {

// Format filesystem.
bool Mkfs(const Partition& partition);

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_FORMAT_H
