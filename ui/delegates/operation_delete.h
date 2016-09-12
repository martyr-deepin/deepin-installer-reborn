// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_DELEGATES_OPERATION_DELETE_H
#define DEEPIN_INSTALLER_REBORN_UI_DELEGATES_OPERATION_DELETE_H

#include "ui/delegates/operation.h"

namespace ui {

class OperationDelete : public Operation {
 public:
  OperationDelete(const QString& device_path,
                  const partman::Partition& partition_orig);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_DELEGATES_OPERATION_DELETE_H
