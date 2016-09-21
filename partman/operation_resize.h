// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_OPERATION_RESIZE_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_OPERATION_RESIZE_H

#include "partman/operation.h"

namespace partman {

// To mark extended partition is resized.
class OperationResize : public Operation {
 public:
  OperationResize(const Partition& partition_orig,
                  const Partition& partition_new);

  void applyToVisual(PartitionList& partitions) const override;
};

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_OPERATION_RESIZE_H
