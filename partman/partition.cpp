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

qint64 Partition::getByteLength() const {
  const qint64 sectors = getSectorLength();
  if (sectors >= 0) {
    return sectors * sector_size;
  } else {
    return -1;
  }
}

qint64 Partition::getSector() const {
  return sector_start + (sector_end - sector_start) / 2;
}

qint64 Partition::getSectorLength() const {
  if (sector_start >= 0 && sector_end >= 0) {
    return sector_end - sector_start + 1;
  } else {
    return -1;
  }
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