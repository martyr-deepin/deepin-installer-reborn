// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_OPERATION_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_OPERATION_H

#include <QList>

#include "partman/device.h"

namespace ui {

enum class OperationType {
  Create,
  Delete,
  Format,
};

class Operation {
 public:
  Operation();
  virtual ~Operation() {}

  QString device_path;
  OperationType type;
  partman::Partition partition_original;
  partman::Partition partition_new;
};

typedef QList<Operation> OperationList;

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_OPERATION_H
