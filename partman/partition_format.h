// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_PARTMAN_PARTITION_FORMAT_H
#define INSTALLER_PARTMAN_PARTITION_FORMAT_H

#include <QString>

#include "partman/partition.h"

namespace installer {

// Format filesystem.
bool Mkfs(const Partition& partition);

}  // namespace installer

#endif  // INSTALLER_PARTMAN_PARTITION_FORMAT_H
