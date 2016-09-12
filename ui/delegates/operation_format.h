// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_DELEGATES_OPERATION_FORMAT_H
#define DEEPIN_INSTALLER_REBORN_UI_DELEGATES_OPERATION_FORMAT_H

#include "ui/delegates/operation.h"

namespace ui {

// To mark an existing partition is formated.
class OperationFormat : public Operation {
 public:
  OperationFormat(const QString& device_path,
                  const partman::Partition& partition_orig,
                  const partman::Partition& partition_new);
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_DELEGATES_OPERATION_FORMAT_H
