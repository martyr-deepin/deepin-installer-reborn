// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/operation.h"

#include "partman/libparted_util.h"
#include "partman/partition_format.h"

namespace installer {

Operation::Operation(OperationType type,
                     const Partition& orig_partition,
                     const Partition& new_partition)
    : type(type),
      orig_partition(orig_partition),
      new_partition(new_partition) {
}

Operation::~Operation() {
}

bool Operation::applyToDisk() const {
  bool ok;
  switch (type) {
    case OperationType::Create: {
      ok = CreatePartition(new_partition);
      if (ok) {
        ok = Mkfs(new_partition);
      }
      break;
    }

    case OperationType::Delete: {
      ok = DeletePartition(orig_partition);
      break;
    }

    case OperationType::Format: {
      ok = SetPartitionType(new_partition);
      if (ok) {
        ok = Mkfs(new_partition);
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
  switch (type) {
    case OperationType::Create: {
      this->applyCreateVisual(partitions);
      break;
    }
    case OperationType::Delete: {
      this->applyDeleteVisual(partitions);
      break;
    }
    case OperationType::Format: {
      this->substitute(partitions);
      break;
    }
    case OperationType::MountPoint: {
      this->substitute(partitions);
      break;
    }
    default: {
      break;
    }
  }
}

void Operation::applyCreateVisual(PartitionList& partitions) const {
  const int index = PartitionIndex(partitions, orig_partition);
  // FIXME(xushaohua): handles index == -1.
//      Q_ASSERT(index > -1);
  if (index < 0) {
    return;
  }

  if (new_partition.sectors_unallocated_succeeding > 0) {
    // Create an unallocated partition after this one.
    Partition succeeding_partition;
    succeeding_partition.device_path = new_partition.device_path;
    succeeding_partition.sector_end = orig_partition.sector_end;
    succeeding_partition.sector_start = succeeding_partition.sector_end -
                                        new_partition.sectors_unallocated_succeeding;
    if (index+1 == partitions.length()) {
      partitions.append(succeeding_partition);
    } else {
      partitions.insert(index+1, succeeding_partition);
    }
  }

  partitions[index] = new_partition;

  if (new_partition.sectors_unallocated_preceding > 0) {
    // Create an unallocated partition before this one.
    Partition preceding_partition;
    preceding_partition.device_path = new_partition.device_path;
    preceding_partition.sector_start = orig_partition.sector_start;
    preceding_partition.sector_end = preceding_partition.sector_start +
                                     new_partition.sectors_unallocated_preceding;
    if (index == 0) {
      partitions.prepend(preceding_partition);
    } else {
      partitions.insert(index, preceding_partition);
    }
  }
}

void Operation::applyDeleteVisual(PartitionList& partitions) const {
  int index = PartitionIndex(partitions, orig_partition);
  Partition empty_partition = new_partition;

  if (index > 0 &&
      (partitions.at(index - 1).type == PartitionType::Unallocated)) {
    // Not the first partition, try to merge with previous one.
    empty_partition.sector_start = partitions.at(index - 1).sector_start;
    partitions.removeAt(index - 1);
    index -= 1;
  }

  if (index < partitions.length() - 1 &&
      (partitions.at(index + 1).type == PartitionType::Unallocated)) {
    // Not the last partition, try to merge with next partition.
    empty_partition.sector_end = partitions.at(index + 1).sector_end;
    partitions[index] = empty_partition;
    partitions.removeAt(index + 1);
  }
  partitions[index] = empty_partition;
}

void Operation::substitute(PartitionList& partitions) const {
  const int index = PartitionIndex(partitions, orig_partition);
  // FIXME(xushaohua): handles index == -1.
//  Q_ASSERT(index > -1);

  if (index > -1) {
    partitions[index] = new_partition;
  }
}

}  // namespace installer