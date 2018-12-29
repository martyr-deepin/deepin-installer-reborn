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

#ifndef INSTALLER_PARTMAN_PARTITION_H
#define INSTALLER_PARTMAN_PARTITION_H

#include <QDebug>
#include <QList>
#include <QStringList>
#include <QSharedPointer>

#include "partman/fs.h"
#include "partman/structs.h"

namespace installer {

enum class PartitionType {
  Normal,
  Logical,
  Extended,
  Unallocated,
};
QDebug& operator<<(QDebug& debug, const PartitionType& partition_type);

// Real <-> New
// Real <-> Format
// Real <-> Delete
enum class PartitionStatus {
  Real,  // Both partition range and fs type are not changed
  New,  // Both partition range and fs type are changed.
  Format,  // Only fs type of partition is changed.
  Delete,  // Partition is marked to delete.
};
QDebug& operator<<(QDebug& debug, const PartitionStatus& partition_status);

// Refers PedPartitionFlag
enum class PartitionFlag {
  Nil,  // Used as a placeholder, with value 0.
  Boot,
  Root,
  Swap,
  Hidden,
  RAID,
  LVM,
  LBA,
  HPServer,
  PALO,
  PREP,
  MSFTReserved,
  BIOSGrub,
  AppleTVRecovery,
  Diag,
  LegacyBoot,
  MSFTData,
  IRST,
  ESP,
};
QDebug& operator<<(QDebug& debug, const PartitionFlag& flag);
typedef QList<PartitionFlag> PartitionFlags;

class Partition {
 public:
  Partition();
  Partition(const Partition &partition);
  ~Partition();

  typedef QSharedPointer<Partition> Ptr;

  bool operator==(const Partition& other) const;

  QString device_path;
  QString path;
  QString label;  // Filesystem label.
  QString name;  // Only some partition tables support part-label.
  int partition_number;
  PartitionType type;
  PartitionStatus status;
  FsType fs;
  OsType os;
  bool busy;  // Set to true when this partition is being used.

  // sector size of the disk device needed for converting to/from sectors
  // and bytes
  qint64 sector_size;

  qint64 length;
  qint64 freespace;

  qint64 start_sector;
  qint64 end_sector;

  // |mount_point| is not system mount point, it is set by user
  // in partition page.
  QString mount_point;

  // Partition flags, like "boot", "esp", "raid" and "lvm".
  PartitionFlags flags;

  // Change partition number and partition path.
  void changeNumber(int partition_number);

  // Returns actual partition size.
  // |length| is set only if this partition has a filesystem.
  qint64 getByteLength() const;

  // Returns a sector within this partition.
  // This method is only useful when calling ped_disk_get_partition_by_sector()
  qint64 getSector() const;

  // Returns length in sector.
  qint64 getSectorLength() const;
};
QDebug& operator<<(QDebug& debug, const Partition& partition);
QDebug& operator<<(QDebug& debug, const Partition::Ptr partition);

typedef QList<Partition::Ptr> PartitionList;

// Get index of extended partition in |partitions|. Returns -1 if not found.
int ExtendedPartitionIndex(const PartitionList& partitions);

// Get all of primary partitions(including extended partition) in |partitions|.
PartitionList GetPrimaryPartitions(const PartitionList& partitions);

// Get all of logical partitions in |partition|.
PartitionList GetLogicalPartitions(const PartitionList& partitions);

// Check whether two partitions are intersected.
bool IsPartitionsJoint(const Partition::Ptr part1, const Partition::Ptr part2);

// Get index of |partition| in |partitions|. Returns -1 if not found.
int PartitionIndex(const PartitionList& partitions, const Partition::Ptr partition);

}  // namespace installer

#endif  // INSTALLER_PARTMAN_PARTITION_H
