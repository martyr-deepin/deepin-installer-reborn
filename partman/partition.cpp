/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "partman/partition.h"

namespace installer {

QDebug& operator<<(QDebug& debug, const PartitionType& partition_type) {
  QString type;
  switch (partition_type) {
    case PartitionType::Normal: {
      type = "Normal";
      break;
    }
    case PartitionType::Logical: {
      type = "Logical";
      break;
    }
    case PartitionType::Extended: {
      type = "Extended";
      break;
    }
    case PartitionType::Unallocated: {
      type = "Unallocated";
      break;
    }
  }
  debug << type;
  return debug;
}

QDebug& operator<<(QDebug& debug, const PartitionStatus& partition_status) {
  QString status;
  switch (partition_status) {
    case PartitionStatus::Format: {
      status = "Format";
      break;
    }
    case PartitionStatus::New: {
      status = "New";
      break;
    }
    case PartitionStatus::Real: {
      status = "Real";
      break;
    }
    case PartitionStatus::Delete: {
      status = "Delete";
      break;
    }
    default: {
      break;
    }
  }
  debug << status;
  return debug;
}

QDebug& operator<<(QDebug& debug, const PartitionFlag& flag) {
  debug << static_cast<int>(flag);
  return debug;
}

Partition::Partition()
    : device_path(),
      path(),
      label(),
      name(),
      partition_number(-1),
      type(PartitionType::Unallocated),
      status(PartitionStatus::Real),
      fs(FsType::Empty),
      os(OsType::Empty),
      busy(false),
      sector_size(0),
      length(-1),
      freespace(-1),
      start_sector(-1),
      end_sector(-1),
      mount_point(),
      flags() {
}

Partition::~Partition() {

}

bool Partition::operator==(const Partition& other) const {
  return (this->device_path == other.device_path &&
          this->start_sector == other.start_sector &&
          this->end_sector == other.end_sector &&
          this->sector_size == other.sector_size &&
          type == other.type);
}

void Partition::changeNumber(int partition_number) {
  this->partition_number = partition_number;
  this->path = QString("%1%2").arg(this->device_path).arg(partition_number);
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
  return start_sector + (end_sector - start_sector) / 2;
}

qint64 Partition::getSectorLength() const {
  if (start_sector >= 0 && end_sector >= 0) {
    return end_sector - start_sector + 1;
  } else {
    return -1;
  }
}

QDebug& operator<<(QDebug& debug, const Partition& partition) {
  debug << "Partition: {"
        << "path:" << partition.path
        << "number:" << partition.partition_number
        << "fs:" << partition.fs
        << "type:" << partition.type
        << "status:" << partition.status
        << "os:" << partition.os
        << "busy:" << partition.busy
        << "label:" << partition.label
        << "name:" << partition.name
        << "mount point:" << partition.mount_point
        << "start:" << partition.start_sector
        << "end:" << partition.end_sector
        << "sector size:" << partition.sector_size
        << "flags:" << partition.flags
        << "}";
  return debug;
}

int ExtendedPartitionIndex(const PartitionList& partitions) {
  for (int i = 0; i < partitions.length(); ++i) {
    if (partitions.at(i).type == PartitionType::Extended) {
      return i;
    }
  }
  return -1;
}

PartitionList GetPrimaryPartitions(const PartitionList& partitions) {
  PartitionList result;
  for (const Partition& partition : partitions) {
    if (partition.type == PartitionType::Normal ||
        partition.type == PartitionType::Extended) {
      result.append(partition);
    }
  }
  return result;
}

PartitionList GetLogicalPartitions(const PartitionList& partitions) {
  PartitionList result;
  for (const Partition& partition : partitions) {
    if (partition.type == PartitionType::Logical) {
      result.append(partition);
    }
  }
  return result;
}

bool IsPartitionsJoint(const Partition& part1, const Partition& part2) {
  return ((part1.start_sector <= part2.start_sector &&
          part2.start_sector <= part1.end_sector) ||
         (part1.start_sector <= part2.end_sector &&
          part2.end_sector <= part1.end_sector));
}

int PartitionIndex(const PartitionList& partitions,
                   const Partition& partition) {
  for (int i = 0; i < partitions.length(); ++i) {
    if (partition.type == partitions[i].type &&
        partition.start_sector >= partitions[i].start_sector &&
        partition.end_sector <= partitions[i].end_sector) {
      return i;
    }
  }
  return -1;
}

}  // namespace installer