/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "partman/operation.h"

#include <QDebug>
#include <memory>

#include "partman/libparted_util.h"
#include "partman/partition_format.h"
#include "ui/delegates/partition_util.h"

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
    case OperationType::MountPoint: {
      type = "MountPoint";
      break;
    }
    case OperationType::NewPartTable: {
      type = "NewPartTable";
      break;
    }
    case OperationType::Resize: {
      type = "Resize";
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

Operation::Operation(const Device::Ptr device)
    : type(OperationType::NewPartTable),
      device(device) {
}

Operation::Operation(OperationType type,
                     const Partition::Ptr orig_partition,
                     const Partition::Ptr new_partition)
    : type(type),
      orig_partition(new Partition(*orig_partition)),
      new_partition(new Partition(*new_partition)) {
}

Operation::~Operation() {
}

bool Operation::applyToDisk() {
  switch (type) {
    case OperationType::Create: {
      // Filters filesystem type.
      if (new_partition->fs == FsType::Unknown) {
        qCritical() << "OperationCreate unknown fs" << new_partition;
        return false;
      }

      // Create new partition in disk partition table.
      if (!CreatePartition(new_partition)) {
        qCritical() << "CreatePartition() failed" << new_partition;
        return false;
      }

      // Update partition number and path.
      if (!UpdatePartitionNumber(new_partition)) {
        qCritical() << "OperationCreate UpdatePartitionNumber() failed:"
                    << new_partition;
        return false;
      }

      if (!CommitUdevEvent(new_partition->path)) {
        qCritical() << "No device found:" << new_partition->path;
      }

      // Ignores extended partition. And check filesystem type.
      if ((new_partition->type != PartitionType::Extended) &&
          (new_partition->fs != FsType::Empty)) {
        // Create new filesystem on new_partition.
        if (!Mkfs(new_partition)) {
          qCritical() << "OperationCreate Mkfs() failed:" << new_partition;
          return false;
        }

        // Set flags on new_partition.
        if (!SetPartitionFlags(new_partition)) {
          qCritical() << "OperationCreate SetPartitionFlags() failed:"
                      << new_partition;
          return false;
        }
      }
      return true;
    }

    case OperationType::Delete: {
      // Delete partition from disk partition table.
      if (!DeletePartition(orig_partition)) {
        qCritical() << "DeletePartition() failed:" << orig_partition;
        return false;
      } else {
        return true;
      }
    }

    case OperationType::Format: {
      // Filters filesystem type.
      if (new_partition->fs == FsType::Unknown) {
        qCritical() << "OperationFormat unknown fs" << new_partition;
        return false;
      }

      // Update filesystem type in partition table.
      if (!SetPartitionType(new_partition)) {
        qCritical() << "OperationFormat SetPartitionType() failed:"
                    << new_partition;
        return false;
      }

      // Update partition number and path.
      if (!UpdatePartitionNumber(new_partition)) {
        qCritical() << "OperationFormat UpdatePartitionNumber() failed:"
                    << new_partition;
        return false;
      }

      if (new_partition->fs != FsType::Empty) {
        // Create new filesystem.
        if (!Mkfs(new_partition)) {
          qCritical() << "OperationFormat Mkfs() failed:" << new_partition;
          return false;
        }

        // Set flags of new_partition.
        if (!SetPartitionFlags(new_partition)) {
          qCritical() << "OperationFormat SetPartitionFlags() failed:"
                      << new_partition;
          return false;
        }
      }

      return true;
    }

    case OperationType::Invalid: {
      qCritical() << "Invalid operation!";
      return false;
    }

    case OperationType::MountPoint: {
      // Update flags of new_partition.
      if (!SetPartitionFlags(new_partition)) {
        qCritical() << "SetPartitionFlags() failed:" << new_partition;
        return false;
      } else {
        return true;
      }
    }

    case OperationType::NewPartTable: {
      if (!CreatePartitionTable(device->path, device->table)) {
        qCritical() << "CreatePartitionTable() failed:" << device.data();
        return false;
      } else {
        return true;
      }
    }

    case OperationType::Resize: {
      // Resize extended partition.
      if (!ResizeMovePartition(new_partition)) {
        qCritical() << "ResizeMovePartition() failed:" << new_partition;
        return false;
      } else {
        return true;
      }
    }

    default: {
      qCritical() << "Unknown type of operation:" << type;
      return false;
    }
  }
}

void Operation::applyToVisual(const Device::Ptr device) const {
  PartitionList& partitions = device->partitions;
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
    case OperationType::NewPartTable: {
      this->applyNewTableVisual(device);
      break;
    }
    case OperationType::Resize: {
      this->applyResizeVisual(partitions);
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
      if (new_partition->type == PartitionType::Extended) {
        desc = QObject::tr("Create extended partition %1")
            .arg(new_partition->path);
      }
      else if (new_partition->mount_point.isEmpty()) {
        desc = QObject::tr("Create new partition %1, type: %2")
            .arg(new_partition->path)
            .arg(GetFsTypeName(new_partition->fs));
      } else {
        desc = QObject::tr("Create new partition %1 as %2 (mountpoint), "
                           "type: %3")
            .arg(new_partition->path)
            .arg(new_partition->mount_point)
            .arg(GetFsTypeName(new_partition->fs));
      }
      break;
    }
    case OperationType::Delete: {
      desc = QObject::tr("Delete %1 partition").arg(orig_partition->path);
      break;
    }
    case OperationType::Format: {
      if (new_partition->mount_point.isEmpty()) {
        desc = QObject::tr("Format %1 partition, type: %2")
            .arg(new_partition->path)
            .arg(GetFsTypeName(new_partition->fs));
      } else {
        desc = QObject::tr("Format %1 partition as %2 (mountpoint), type: %3")
            .arg(new_partition->path)
            .arg(new_partition->mount_point)
            .arg( GetFsTypeName(new_partition->fs));
      }
      break;
    }
    case OperationType::MountPoint: {
      desc = QObject::tr("Use %1 partition as %2 (mountpoint)")
          .arg(new_partition->path)
          .arg(new_partition->mount_point);
      break;
    }
    case OperationType::NewPartTable: {
      desc = QObject::tr("Format %1 and create %2 new partition table")
          .arg(device->path)
          .arg(GetPartTableName(device->table));
      break;
    }
    case OperationType::Resize: {
      desc = QObject::tr("Adjust the size of %1 partition")
          .arg(new_partition->path);
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
  // Policy:
  // * Create unallocated partition if orig_partition is larger than
  //   new_partition

  qDebug() << "applyCreateVisual(), partitions:" << partitions
           << "orig partition:" << *orig_partition.data()
           << ", new_partition:" << *new_partition.data();
  int index = PartitionIndex(partitions, orig_partition);
  // FIXME(xushaohua): index == -1
  if (index == -1) {
    qCritical() << "applyCreateVisual() Failed to find partition:"
                << orig_partition->path;
    return;
  }

  // Do not remove orig partition when creating extended partition.
  if (new_partition->type == PartitionType::Extended) {
    partitions.insert(index, new_partition);
    // Extended partition does not consume any real sectors, so no need
    // to insert unallocated partitions. Just leave orig_partition unchanged.
    return;
  } else {
    partitions[index] = new_partition;
  }

  const qint64 twoMebiByteSector = 2 * kMebiByte / orig_partition->sector_size;

  // Gap between orig_partition.start <-> new_partition.start.
  if (new_partition->start_sector - orig_partition->start_sector > twoMebiByteSector) {
      Partition::Ptr unallocated(new Partition);
      unallocated->device_path  = orig_partition->device_path;
      unallocated->sector_size  = orig_partition->sector_size;
      unallocated->type         = PartitionType::Unallocated;
      unallocated->start_sector = orig_partition->start_sector + 1;
      unallocated->end_sector   = new_partition->start_sector - 1;
      partitions.insert(index, unallocated);
      index += 1;
  }

  // Gap between new_partition.end <-> orig_partition.end
  if (orig_partition->end_sector - new_partition->end_sector > twoMebiByteSector) {
      Partition::Ptr unallocated(new Partition);
      unallocated->device_path  = orig_partition->device_path;
      unallocated->sector_size  = orig_partition->sector_size;
      unallocated->type         = PartitionType::Unallocated;
      unallocated->start_sector = new_partition->end_sector + 1;
      unallocated->end_sector   = orig_partition->end_sector - 1;
      if (index + 1 == partitions.length()) {
          partitions.append(unallocated);
      }
      else {
          partitions.insert(index + 1, unallocated);
      }
  }

  MergeUnallocatedPartitions(partitions);
}

void Operation::applyDeleteVisual(PartitionList& partitions) const {
  qDebug() << Q_FUNC_INFO << *orig_partition << *new_partition;
  this->substitute(partitions);
  MergeUnallocatedPartitions(partitions);
}

void Operation::applyNewTableVisual(const Device::Ptr device) const {
  device->table = this->device->table;
  device->partitions.clear();
  Partition::Ptr free_partition (new Partition);
  free_partition->device_path = device->path;
  free_partition->path = "";
  free_partition->partition_number = -1;
  free_partition->start_sector = 1;
  free_partition->end_sector = device->length - (device->table == PartitionTableType::GPT ? 33 : 0);
  free_partition->sector_size = device->sector_size;
  free_partition->type = PartitionType::Unallocated;
  device->partitions.append(free_partition);

  // Update max primary partition number.
  if (device->table == PartitionTableType::MsDos) {
    device->max_prims = kMsDosPartitionNums;
  } else if (device->table == PartitionTableType::GPT) {
    device->max_prims = kGPTPartitionNums;
  }
}

void Operation::applyResizeVisual(PartitionList& partitions) const {
  // Currently only extended partition is allowed to resize
  if (new_partition->type == PartitionType::Extended) {
    this->substitute(partitions);
  }
}

void Operation::substitute(PartitionList& partitions) const {
  const int index = PartitionIndex(partitions, orig_partition);
  if (index == -1) {
    qCritical() << "substitute() Failed to find partition:"
                << orig_partition->path;
  } else {
    partitions[index] = new_partition;
  }
}

QDebug& operator<<(QDebug& debug, const Operation& operation) {
    debug << "Operation: {" << endl
    << "    type: " << operation.type << endl
    << "    opri_partion: " << operation.orig_partition << endl
    << "    new_partition: " << operation.new_partition << endl
    << "}";
    return debug;
}

void MergeOperations(OperationList& operations, const Operation& operation) {
  Q_UNUSED(operations);
  Q_UNUSED(operation);
}

void MergeUnallocatedPartitions(PartitionList& partitions) {
  // Do not handles empty partition list.
  if (partitions.isEmpty()) {
    return;
  }

  // Looks for gaps in between.
  int global_index = 0;
  while (global_index < partitions.length()) {
    int index;
    // Find unallocated partition.
    for (index = global_index; index < partitions.length(); ++ index) {
      if (partitions.at(index)->type == PartitionType::Unallocated) {
        break;
      }
    }

    // No more unallocated partitions.
    if (index >= partitions.length()) {
      break;
    }

    global_index = index;
    // Find all connected unallocated partitions
    while ((index + 1) < partitions.length()) {
      const Partition::Ptr next_part = partitions.at(index + 1);
      if (next_part->type == PartitionType::Unallocated) {
        partitions[global_index]->end_sector = next_part->end_sector;
        partitions.removeAt(index + 1);
      } else if (next_part->type == PartitionType::Extended) {
        // Ignores extended partition
        ++ index;
      } else {
        // Breaks if next_part is logical partition or normal partition.
        break;
      }
    }

    ++ global_index;
  }
}

}  // namespace installer
