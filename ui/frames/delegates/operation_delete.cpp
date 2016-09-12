// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/delegates/operation_delete.h"

namespace ui {

OperationDelete::OperationDelete(const QString& device_path,
                                 const partman::Partition& partition_orig) {
  this->type = OperationType::Delete;
  this->device_path = device_path;
  this->partition_orig = partition_orig;
}

}  // namespace ui