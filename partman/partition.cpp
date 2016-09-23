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

bool Partition::operator==(const Partition& other) const {
  return (this->device_path == other.device_path &&
          this->partition_number == other.partition_number &&
          this->sector_start == other.sector_start &&
          type == other.type);
}

qint64 Partition::getLength() const {
  return sector_size * sectors_total;
}

qint64 Partition::getSector() const {
  return sector_start + (sector_end - sector_start) / 2;
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
  sectors_unallocated = 0;
  flags.clear();
}

}  // namespace partman