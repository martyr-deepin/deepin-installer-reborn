// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/operation.h"

#include "partman/libparted_util.h"
#include "partman/partition_format.h"

namespace installer {

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
  bool ok;
  switch (type) {
    case OperationType::Create: {
      ok = CreatePartition(partition_new);
      if (ok) {
        ok = Mkfs(partition_new);
      }
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
  switch (type) {
    case OperationType::Create: {
      const int index = findIndexOriginal(partitions);
      // FIXME(xushaohua): handles index == -1.
//      Q_ASSERT(index > -1);
      if (index < 0) {
        break;
      }

      if (partition_new.sectors_unallocated_succeeding > 0) {
        // Create an unallocated partition after this one.
        Partition succeeding_partition;
        succeeding_partition.device_path = partition_new.device_path;
        succeeding_partition.sector_end = partition_orig.sector_end;
        succeeding_partition.sector_start = succeeding_partition.sector_end -
            partition_new.sectors_unallocated_succeeding;
        if (index+1 == partitions.length()) {
          partitions.append(succeeding_partition);
        } else {
          partitions.insert(index+1, succeeding_partition);
        }
      }

      partitions[index] = partition_new;

      if (partition_new.sectors_unallocated_preceding > 0) {
        // Create an unallocated partition before this one.
        Partition preceding_partition;
        preceding_partition.device_path = partition_new.device_path;
        preceding_partition.sector_start = partition_orig.sector_start;
        preceding_partition.sector_end = preceding_partition.sector_start +
            partition_new.sectors_unallocated_preceding;
        if (index == 0) {
          partitions.prepend(preceding_partition);
        } else {
          partitions.insert(index, preceding_partition);
        }
      }

      break;
    }
    case OperationType::Delete: {
      int index = findIndexOriginal(partitions);
      Partition empty_partition = partition_new;

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
  const int index = findIndexOriginal(partitions);
  // FIXME(xushaohua): handles index == -1.
//  Q_ASSERT(index > -1);

  if (index > -1) {
    partitions[index] = partition_new;
  }
}

}  // namespace installer