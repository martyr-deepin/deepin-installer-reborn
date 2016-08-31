// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partman/partition.h"

namespace partman {

Partition::Partition() {
  this->reset();
}

Partition::~Partition() {

}

void Partition::reset() {
  device_path = "";
  uuid = "";
  name = "";
  partition_number = -1;
  whole_device = false;
  type = PartitionType::Unallocated;
  status = PartitionStatus::Real;
  sector_start = -1;
  sector_end = -1;
  sectors_used = -1;
  sectors_unused = -1;
  sectors_unallocated = -1;
  flags.clear();
  logicals.clear();
}

}  // namespace partman