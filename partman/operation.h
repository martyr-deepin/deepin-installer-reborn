// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_OPERATION_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_OPERATION_H

#include <QList>

#include "partman/device.h"

namespace partman {

enum class OperationType {
  Create,
  Delete,
  Format,
  MountPoint,
  Resize,
  Invalid,  // operation type not set
};

// Abstract class for operations.
class Operation {
 public:
  Operation(OperationType type,
            const Partition& partition_orig,
            const Partition& partition_new);
  ~Operation();

  OperationType type;
  Partition partition_orig;
  Partition partition_new;

  // Apply operation by updating partition list.
  void applyToVisual(PartitionList& partitions);

 private:
  int findIndexNew(const PartitionList& partitions) const;
  int findIndexOriginal(const PartitionList& partitions) const;

  // For operations which do not change partition boundaries.
  void substitute(PartitionList& partitions) const;
};

typedef QList<Operation> OperationList;

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_OPERATION_H
