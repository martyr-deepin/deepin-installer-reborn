// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_H

#include <QList>
#include <QStringList>

#include "partman/fs.h"
#include "partman/structs.h"

namespace partman {

enum class PartitionType {
  Primary,
  Logical,
  Extended,
  Unallocated,
};

enum class PartitionStatus {
  Real,
  New,
  Formatted,
};

enum class PartitionAlignment {
  Cylinder,  // Align to nearest cylinder
  MebiByte,  // Align to nearest mebibyte
  Strict,  // Strict alignment, no rounding.
};

class Partition {
 public:
  Partition();
  ~Partition();

  bool operator==(const Partition& other) const;
  void reset();

  QString device_path;
  QString path;
  QString uuid;
  QString name;
  QString label;
  int partition_number;
  bool whole_device;
  PartitionType type;
  PartitionStatus status;
  FsType fs;
  OsType os;

  // sector size of the disk device needed for converting to/from sectors
  // and bytes
  qint64 sector_size;

  qint64 length;
  qint64 freespace;

  qint64 sector_start;
  qint64 sector_end;
  qint64 sectors_total;  // sectors_total = sectors_end - sectors_start + 1;
  qint64 sectors_used;
  qint64 sectors_unused;
  qint64 sectors_unallocated;
  qint64 free_space_before;  // Free space preceding partition value
  bool inside_extended;
  bool busy;
  QStringList flags;

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

typedef QList<Partition> PartitionList;

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_H
