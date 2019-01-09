/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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

Partition::Partition(const Partition &partition)
    : device_path(partition.device_path),
      path(partition.path),
      label(partition.label),
      name(partition.name),
      partition_number(partition.partition_number),
      type(partition.type),
      status(partition.status),
      fs(partition.fs),
      os(partition.os),
      busy(partition.busy),
      sector_size(partition.sector_size),
      length(partition.length),
      freespace(partition.freespace),
      start_sector(partition.start_sector),
      end_sector(partition.end_sector),
      mount_point(partition.mount_point),
      flags(partition.flags) {
}

Partition::~Partition() {

}

bool Partition::operator==(const Partition &other) const {
  qDebug() << "===============";
  qDebug() << this->device_path << this->start_sector << this->end_sector << this->path;
  qDebug() << other.device_path << other.start_sector << other.end_sector << other.path;
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
  debug << "Partition: {" << endl
        << "    path:" << partition.path << endl
        << "    number:" << partition.partition_number << endl
        << "    fs:" << partition.fs << endl
        << "    type:" << partition.type << endl
        << "    status:" << partition.status << endl
        << "    os:" << partition.os << endl
        << "    busy:" << partition.busy << endl
        << "    label:" << partition.label << endl
        << "    name:" << partition.name << endl
        << "    mount point:" << partition.mount_point << endl
        << "    start:" << partition.start_sector << endl
        << "    end:" << partition.end_sector << endl
        << "    sector size:" << partition.sector_size << endl
        << "    flags:" << partition.flags << endl
        << "}" << endl;
  return debug;
}

QDebug& operator<<(QDebug& debug, const Partition::Ptr partition) {
  if (partition.isNull())
    debug << "a partition with null value: " << partition.data();
  else
    debug << "Partition: {" << endl
          << "    path:" << partition->path << endl
          << "    number:" << partition->partition_number << endl
          << "    fs:" << partition->fs << endl
          << "    type:" << partition->type << endl
          << "    status:" << partition->status << endl
          << "    os:" << partition->os << endl
          << "    busy:" << partition->busy << endl
          << "    label:" << partition->label << endl
          << "    name:" << partition->name << endl
          << "    mount point:" << partition->mount_point << endl
          << "    start:" << partition->start_sector << endl
          << "    end:" << partition->end_sector << endl
          << "    sector size:" << partition->sector_size << endl
          << "    flags:" << partition->flags << endl
          << "    }" << endl;
  return debug;
}

int ExtendedPartitionIndex(const PartitionList& partitions) {
  for (int i = 0; i < partitions.length(); ++i) {
    if (partitions.at(i)->type == PartitionType::Extended) {
      return i;
    }
  }
  return -1;
}

PartitionList GetPrimaryPartitions(const PartitionList& partitions) {
  PartitionList result;
  for (const Partition::Ptr partition : partitions) {
    if (partition->type == PartitionType::Normal ||
        partition->type == PartitionType::Extended) {
      result.append(partition);
    }
  }
  return result;
}

PartitionList GetLogicalPartitions(const PartitionList& partitions) {
  PartitionList result;
  for (const Partition::Ptr partition : partitions) {
    if (partition->type == PartitionType::Logical) {
      result.append(partition);
    }
  }
  return result;
}

bool IsPartitionsJoint(const Partition::Ptr part1, const Partition::Ptr part2) {
  return ((part1->start_sector <= part2->start_sector &&
          part2->start_sector <= part1->end_sector) ||
         (part1->start_sector <= part2->end_sector &&
          part2->end_sector <= part1->end_sector));
}

int PartitionIndex(const PartitionList& partitions,
                   const Partition::Ptr partition) {
    auto find_it = std::find_if(
      // FIXME: end_sector has a boundary value
        partitions.begin(), partitions.end(), [=](const Partition::Ptr ptr) {
            return (partition->start_sector >= ptr->start_sector &&
                    partition->end_sector - 1 <= ptr->end_sector &&
                    partition->type == ptr->type);
        });

    if (find_it == partitions.end()) {
        return -1;
    }

    return find_it - partitions.begin();
}

}  // namespace installer
