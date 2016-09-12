// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_DELEGATES_OPERATION_MOUNT_POINT_H
#define DEEPIN_INSTALLER_REBORN_UI_DELEGATES_OPERATION_MOUNT_POINT_H

#include "operation.h"

namespace ui {

// To mark mount-point changed in partition
class OperationMountPoint : public Operation {
 public:
  OperationMountPoint(const QString& device_path,
                      const partman::Partition& partition_orig,
                      const partman::Partition& partition_new);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_DELEGATES_OPERATION_MOUNT_POINT_H
