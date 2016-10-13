// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/operation.h"

#include <QDebug>

#include "partman/libparted_util.h"
#include "partman/partition_format.h"

namespace installer {

Operation::Operation(OperationType type,
                     const Partition& orig_partition,
                     const Partition& new_partition)
    : type(type),
      orig_partition(orig_partition),
      new_partition(new_partition) {
#ifndef NDEBUG
  qDebug() << "Operation::constructor()" << orig_partition.path;
  switch (type) {
    case OperationType::Create: {
      qDebug() << "Create!";
      break;
    }
    case OperationType::Delete: {
      qDebug() << "Delete!";
      break;
    }
    case OperationType::Format: {
      qDebug() << "Format!";
      break;
    }
    case OperationType::Resize: {
      qDebug() << "Resize";
      break;
    }
    case OperationType::MountPoint: {
      qDebug() << "MountPoint!";
      break;
    }
    case OperationType::Invalid: {
      qDebug() << "Invalid!";
      break;
    }
  }
#endif
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
  if (index == -1) {
    qCritical() << "applyCreateVisual() Failed to find partition:"
                << orig_partition.path;
    return;
  }

  if (new_partition.sectors_unallocated_succeeding > 0) {
    // Create an unallocated partition after this one.
    Partition succeeding_partition;
    succeeding_partition.device_path = new_partition.device_path;
    succeeding_partition.end_sector = orig_partition.end_sector;
    succeeding_partition.start_sector = succeeding_partition.end_sector -
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
    preceding_partition.start_sector = orig_partition.start_sector;
    preceding_partition.end_sector = preceding_partition.start_sector +
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

//  if (orig_partition.type == PartitionType::Extended) {
//    // Remove extended partition.
//    partitions.removeAt(index);
//    return;
//  }

  Partition empty_partition = new_partition;

  if (index > 0 &&
      (partitions.at(index - 1).type == PartitionType::Unallocated)) {
    // Not the first partition, try to merge with previous freespace partition.
    empty_partition.start_sector = partitions.at(index - 1).start_sector;
    partitions.removeAt(index - 1);
    index -= 1;
  }

  if (index < partitions.length() - 1 &&
      (partitions.at(index + 1).type == PartitionType::Unallocated)) {
    // Not the last partition, try to merge with next freespace partition.
    empty_partition.end_sector = partitions.at(index + 1).end_sector;
    partitions[index] = empty_partition;
    partitions.removeAt(index + 1);
  }
  partitions[index] = empty_partition;
}

void Operation::substitute(PartitionList& partitions) const {
  const int index = PartitionIndex(partitions, orig_partition);
  if (index == -1) {
    qCritical() << "substitute() Failed to find partition:"
                << orig_partition.path;
  } else {
    partitions[index] = new_partition;
  }
}

}  // namespace installer