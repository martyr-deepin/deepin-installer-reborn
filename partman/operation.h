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
  Partition orig_partition;
  Partition new_partition;

  // Apply changes to disk. Returns operation status.
  // Note that this method shall be called in the background thread.
  bool applyToDisk() const;

  // Apply operation by updating partition list.
  void applyToVisual(PartitionList& partitions) const;

  // Get description of this operation.
  QString description() const;

 private:
  void applyCreateVisual(PartitionList& partitions) const;
  void applyDeleteVisual(PartitionList& partitions) const;
  void applyResizeVisual(PartitionList& partitions) const;

  // For operations which do not change partition boundaries.
  void substitute(PartitionList& partitions) const;
};
QDebug& operator<<(QDebug& debug, const Operation& operation);

typedef QList<Operation> OperationList;

}  // namespace installer

#endif  // INSTALLER_PARTMAN_OPERATION_H
