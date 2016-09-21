// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_OPERATION_FORMAT_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_OPERATION_FORMAT_H

#include "partman/operation.h"

namespace partman {

// To mark an existing partition is formated.
class OperationFormat : public Operation {
 public:
  OperationFormat(const Partition& partition_orig,
                  const Partition& partition_new);

  void applyToVisual(PartitionList& partitions) const override;
};

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_OPERATION_FORMAT_H
