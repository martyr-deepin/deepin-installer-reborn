// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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
  Operation(OperationType type,
            const Partition& orig_partition,
            const Partition& new_partition);
  ~Operation();

  OperationType type;

  // This property is used only for NewPartTable. When partition table of
  // a device is changed, all the partitions in it will be cleared first.
  // And partition table is updated too.
  // Device path, sector size and other properties remains unchanged.
  Device device;

  Partition orig_partition;
  Partition new_partition;

  // Apply changes to disk. Returns operation status.
  // Note that this method shall be called in the background thread.
  bool applyToDisk() const;

  // Apply operation by updating device properties.
  void applyToVisual(Device& device) const;

  // Get description of this operation.
  QString description() const;

 private:
  void applyCreateVisual(PartitionList& partitions) const;
  void applyDeleteVisual(PartitionList& partitions) const;

  // Update partition table type and clear partition list.
  void applyNewTableVisual(Device& device) const;

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
