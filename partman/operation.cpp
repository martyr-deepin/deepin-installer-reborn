// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/operation.h"

namespace partman {

Operation::Operation() {
}

Operation::~Operation() {
}

int Operation::findIndexNew(const PartitionList& partitions) const {
  for (int i = 0; i < partitions.length(); ++i) {
    if (partition_new.sector_start >= partitions[i].sector_start &&
        partition_new.sector_end <= partitions[i].sector_end) {
      return i;
    }
  }
  return -1;
}

int Operation::findIndexOriginal(const PartitionList& partitions) const {
  for (int i = 0; i < partitions.length(); ++i) {
    if (partition_orig.sector_start >= partitions[i].sector_start &&
        partition_orig.sector_end <= partitions[i].sector_end) {
      return i;
    }
  }
  return -1;
}

void Operation::substitute(PartitionList& partitions) const {
  int index;
  // TODO(xushaohua): Handles extended partition
  index = findIndexOriginal(partitions);
  if (index > -1) {
    partitions[index] = partition_new;
  }
}

}  // namespace partman