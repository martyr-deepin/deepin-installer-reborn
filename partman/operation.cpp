// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/operation.h"

#include <QDebug>

#include "partman/libparted_util.h"
#include "partman/partition_format.h"

namespace installer {

QDebug& operator<<(QDebug& debug, const OperationType& op_type) {
  QString type;
  switch (op_type) {
    case OperationType::Create: {
      type = "Create";
      break;
    }
    case OperationType::Delete: {
      type = "Delete";
      break;
    }
    case OperationType::Format: {
      type = "Format";
      break;
    }
    case OperationType::Resize: {
      type = "Resize";
      break;
    }
    case OperationType::MountPoint: {
      type = "MountPoint";
      break;
    }
    case OperationType::Invalid: {
      type = "Invalid";
      break;
    }
  }
  debug << type;
  return debug;
}

Operation::Operation(OperationType type,
                     const Partition& orig_partition,
                     const Partition& new_partition)
    : type(type),
      orig_partition(orig_partition),
      new_partition(new_partition) {
  qDebug() << "Operation::constructor()" << type << orig_partition
           << new_partition;
}

Operation::~Operation() {
}

bool Operation::applyToDisk() const {
  bool ok;
  switch (type) {
    case OperationType::Create: {
      ok = CreatePartition(new_partition);
      qDebug() << "applyToDisk() create partition:" << ok;
      if (ok) {
        ok = Mkfs(new_partition);
        qDebug() << "applyToDisk() mkfs:" << ok;
      }
      break;
    }

    case OperationType::Delete: {
      ok = DeletePartition(orig_partition);
      qDebug() << "applyToDisk() delete partition" << ok;
      break;
    }

    case OperationType::Format: {
      ok = SetPartitionType(new_partition);
      qDebug() << "applyToDisk() format set partition type:" << ok;
      if (ok) {
        ok = Mkfs(new_partition);
        qDebug() << "applyToDisk() format mkfs():" << ok;
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

QString Operation::description() const {
  QString desc;
  switch (type) {
    case OperationType::Create: {
      if (new_partition.mount_point.isEmpty()) {
        desc = QObject::tr("Create partition %1 with %2")
            .arg(new_partition.path)
            .arg(GetFsTypeName(new_partition.fs));
      } else {
        desc = QObject::tr("Create partition %1 for %2 with %3")
            .arg(new_partition.path)
            .arg(new_partition.mount_point)
            .arg(GetFsTypeName(new_partition.fs));
      }
      break;
    }
    case OperationType::Delete: {
      desc = QObject::tr("Delete partition %1").arg(orig_partition.path);
      break;
    }
    case OperationType::Format: {
      if (new_partition.mount_point.isEmpty()) {
        desc = QObject::tr("Format %1 with %2")
            .arg(new_partition.path)
            .arg(GetFsTypeName(new_partition.fs));
      } else {
        desc = QObject::tr("Format %1 for %2 with %3")
            .arg(new_partition.path)
            .arg(new_partition.mount_point)
            .arg( GetFsTypeName(new_partition.fs));
      }
      break;
    }
    default: {
      // pass
      break;
    }
  }
  return desc;
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
    succeeding_partition.sector_size = orig_partition.sector_size;
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