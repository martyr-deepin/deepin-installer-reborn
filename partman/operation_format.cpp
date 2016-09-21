// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/operation_format.h"

namespace partman {

OperationFormat::OperationFormat(const Partition& partition_orig,
                                 const Partition& partition_new) {
  this->type = OperationType::Format;
  this->partition_orig = partition_orig;
  this->partition_new = partition_new;
}

void OperationFormat::applyToVisual(PartitionList& partitions) const {
  Q_UNUSED(partitions);
}

}  // namespace partman