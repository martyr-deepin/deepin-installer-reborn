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
  LogicalUnallocated,
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

  // TODO(xushaohua): Remove logicals.
  QList<Partition> logicals;
};

typedef QList<Partition> PartitionList;

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_H
