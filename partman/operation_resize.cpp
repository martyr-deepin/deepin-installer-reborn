// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/operation_resize.h"

namespace partman {

OperationResize::OperationResize(const Partition& partition_orig,
                                 const Partition& partition_new) {
  this->type = OperationType::Resize;
  this->partition_orig = partition_orig;
  this->partition_new = partition_new;
}

void OperationResize::applyToVisual(PartitionList& partitions) const {
  Q_UNUSED(partitions);
}

}  // namespace partman