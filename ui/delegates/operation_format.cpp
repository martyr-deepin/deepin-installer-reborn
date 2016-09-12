// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "operation_format.h"

namespace ui {

OperationFormat::OperationFormat(const QString& device_path,
                                 const partman::Partition& partition_orig,
                                 const partman::Partition& partition_new) {
  this->type = OperationType::Format;
  this->device_path = device_path;
  this->partition_orig = partition_orig;
  this->partition_new = partition_new;
}

}  // namespace ui