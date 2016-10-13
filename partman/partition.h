// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_PARTMAN_PARTITION_H
#define INSTALLER_PARTMAN_PARTITION_H

#include <QDebug>
#include <QList>
#include <QStringList>

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

enum class PartitionStatus {
  Real,
  New,
  Formatted,
};
QDebug& operator<<(QDebug& debug, const PartitionStatus& partition_status);

class Partition {
 public:
  Partition();
  ~Partition();

  bool operator==(const Partition& other) const;

  QString device_path;
  QString path;
  QString uuid;
  QString label;
  QString part_label;  // Only some partition tables support part-label.
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

  // No. of sectors not unallocated before/after this partition.
  // These two properties are only used while creating a new partition.
  // If it is a valid value, create an unallocated-partition later.
  qint64 sectors_unallocated_preceding;
  qint64 sectors_unallocated_succeeding;

  // |mount_point| is not system mount point, it is set by user
  // in partition page.
  QString mount_point;

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

typedef QList<Partition> PartitionList;

// Get index of extended partition in |partitions|. Returns -1 if not found.
int ExtendedPartitionIndex(const PartitionList& partitions);

// Get all of primary partitions(including extended partition) in |partitions|.
PartitionList GetPrimaryPartitions(const PartitionList& partitions);

// Get all of logical partitions in |partition|.
PartitionList GetLogicalPartitions(const PartitionList& partitions);

// Get index of |partition| in |partitions|. Returns -1 if not found.
int PartitionIndex(const PartitionList& partitions, const Partition& partition);

}  // namespace installer

#endif  // INSTALLER_PARTMAN_PARTITION_H
