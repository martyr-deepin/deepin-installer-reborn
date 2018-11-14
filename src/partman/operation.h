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

#ifndef INSTALLER_PARTMAN_OPERATION_H
#define INSTALLER_PARTMAN_OPERATION_H

#include <QDebug>
#include <QList>

#include "partman/device.h"

namespace installer {

enum class OperationType {
  Create,
  Delete,
  Format,
  MountPoint,
  NewPartTable,  // Create new partition table.
  Resize,  // Only used for extended partition.
  Invalid,  // Operation type not set.
};
QDebug& operator<<(QDebug& debug, const OperationType& op_type);

// Abstract class for operations.
class Operation {
 public:
  // Create a new operation, with type OperationType::NewPartTable.
  explicit Operation(const Device::Ptr device);

  Operation(OperationType type,
            const Partition::Ptr orig_partition,
            const Partition::Ptr new_partition);
  ~Operation();

  OperationType type;

  // This property is used only for NewPartTable. When partition table of
  // a device is changed, all the partitions in it will be cleared first.
  // And partition table is updated too.
  // Device path, sector size and other properties remains unchanged.
  Device::Ptr device;

  Partition::Ptr orig_partition;
  Partition::Ptr new_partition;

  // Apply changes to disk. Returns operation status.
  // Note that this method shall be called in the background thread.
  bool applyToDisk();

  // Apply operation by updating device properties.
  void applyToVisual(const Device::Ptr device) const;

  // Get description of this operation.
  QString description() const;

 private:
  void applyCreateVisual(PartitionList& partitions) const;
  void applyDeleteVisual(PartitionList& partitions) const;

  // Update partition table type and clear partition list.
  void applyNewTableVisual(const Device::Ptr device) const;

  void applyResizeVisual(PartitionList& partitions) const;

  // For operations which do not change partition boundaries.
  void substitute(PartitionList& partitions) const;
};
QDebug& operator<<(QDebug& debug, const Operation& operation);

typedef QList<Operation> OperationList;

// Merge |operation| in |operations|.
void MergeOperations(OperationList& operations, const Operation& operation);

// Merge unallocated partitions.
void MergeUnallocatedPartitions(PartitionList& partitions);

}  // namespace installer

#endif  // INSTALLER_PARTMAN_OPERATION_H
