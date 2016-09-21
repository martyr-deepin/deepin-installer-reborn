// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/operation_create.h"

namespace partman {

OperationCreate::OperationCreate(const Partition& partition_orig,
                                 const Partition& partition_new) {
  this->type = OperationType::Create;
  this->partition_orig = partition_orig;
  this->partition_new = partition_new;
}

void OperationCreate::applyToVisual(PartitionList& partitions) const {
  // TODO(xushaohua): Calculate partition size and insert unallocated one if
  // needed.
  this->substitute(partitions);
}

}  // namespace partman