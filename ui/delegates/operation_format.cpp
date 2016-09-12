// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/operation_format.h"

namespace ui {

OperationFormat::OperationFormat(const partman::Partition& partition_orig,
                                 const partman::Partition& partition_new) {
  this->type = OperationType::Format;
  this->partition_orig = partition_orig;
  this->partition_new = partition_new;
}

}  // namespace ui