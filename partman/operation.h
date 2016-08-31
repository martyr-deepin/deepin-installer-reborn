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
};

class Operation {
 public:
  Operation();
  virtual ~Operation() {}

  Device device;
  OperationType type;
  Partition partition_original;
  Partition partition_new;
};

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_OPERATION_H
