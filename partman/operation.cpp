// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/operation.h"

#include "partman/libparted_util.h"
#include "partman/partition_format.h"

namespace partman {

Operation::Operation(OperationType type,
                     const Partition& partition_orig,
                     const Partition& partition_new)
    : type(type),
      partition_orig(partition_orig),
      partition_new(partition_new) {
}

Operation::~Operation() {
}

bool Operation::applyToDisk() const {
  bool ok = false;
  switch (type) {
    case OperationType::Create: {
      ok = false;
      break;
    }

    case OperationType::Delete: {
      ok = DeletePartition(partition_orig);
      break;
    }

    case OperationType::Format: {
      ok = SetPartitionType(partition_new);
      if (ok) {
        ok = Mkfs(partition_new);
      }
      break;
    }

    case OperationType::Resize: {
      ok = false;
      break;
    }

    default: {
      ok = false;
      break;
    }
  }

  return ok;
}

void Operation::applyToVisual(PartitionList& partitions) const {
  // TODO(xushaohua): Check operation type.
  this->substitute(partitions);
}

int Operation::findIndexNew(const PartitionList& partitions) const {
  for (int i = 0; i < partitions.length(); ++i) {
    if (partition_new.sector_start >= partitions[i].sector_start &&
        partition_new.sector_end <= partitions[i].sector_end) {
      return i;
    }
  }
  return -1;
}

int Operation::findIndexOriginal(const PartitionList& partitions) const {
  for (int i = 0; i < partitions.length(); ++i) {
    if (partition_orig.sector_start >= partitions[i].sector_start &&
        partition_orig.sector_end <= partitions[i].sector_end) {
      return i;
    }
  }
  return -1;
}

void Operation::substitute(PartitionList& partitions) const {
  int index;
  // TODO(xushaohua): Handles extended partition
  index = findIndexOriginal(partitions);
  if (index > -1) {
    partitions[index] = partition_new;
  }
}

}  // namespace partman