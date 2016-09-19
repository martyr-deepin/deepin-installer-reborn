// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_DELEGATES_OPERATION_H
#define DEEPIN_INSTALLER_REBORN_UI_DELEGATES_OPERATION_H

#include "partman/device.h"

namespace ui {

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

  OperationType type;
  partman::Partition partition_orig;
  partman::Partition partition_new;

  // Apply operation by updating partition list.
  virtual void applyToVisual(partman::PartitionList& partitions) const = 0;

 protected:
  int findIndexNew(const partman::PartitionList& partitions) const;
  int findIndexOriginal(const partman::PartitionList& partitions) const;

  // For operations which do not change partition boundaries.
  void substitute(partman::PartitionList& partitions) const;
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_DELEGATES_OPERATION_H
