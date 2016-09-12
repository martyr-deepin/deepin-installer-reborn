// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/operation_mount_point.h"

namespace ui {

OperationMountPoint::OperationMountPoint(const partman::Partition& partition_orig,
                                         const partman::Partition& partition_new) {
  this->type = OperationType::MountPoint;
  this->partition_orig = partition_orig;
  this->partition_new = partition_new;
}

}  // namespace ui