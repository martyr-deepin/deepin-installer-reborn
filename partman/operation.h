// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_OPERATION_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_OPERATION_H

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
  Operation();
  virtual ~Operation();

  OperationType type;
  Partition partition_orig;
  Partition partition_new;

  // Apply operation by updating partition list.
  virtual void applyToVisual(PartitionList& partitions) const = 0;

 protected:
  int findIndexNew(const PartitionList& partitions) const;
  int findIndexOriginal(const PartitionList& partitions) const;

  // For operations which do not change partition boundaries.
  void substitute(PartitionList& partitions) const;
};

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_OPERATION_H
