// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_DELEGATES_OPERATION_CREATE_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_DELEGATES_OPERATION_CREATE_H

#include "ui/frames/delegates/operation.h"

namespace ui {

// To mark new partition is created
class OperationCreate : public Operation{
 public:
  OperationCreate(const QString& device_path,
                  const partman::Partition& partition_orig,
                  const partman::Partition& partition_new);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_DELEGATES_OPERATION_CREATE_H
