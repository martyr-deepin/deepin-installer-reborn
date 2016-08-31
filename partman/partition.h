// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_H
#define DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_H

#include <QStringList>

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

  void reset();

  QString device_path;
  QString uuid;
  QString name;
  int partition_number;
  bool whole_device;
  PartitionType type;
  PartitionStatus status;

  Sector sector_start;
  Sector sector_end;
  Sector sectors_used;
  Sector sectors_unused;
  Sector sectors_unallocated;
  bool inside_extended;
  bool busy;
  QStringList flags;

  QList<Partition> logicals;
};

}  // namespace partman

#endif  // DEEPIN_INSTALLER_REBORN_PARTMAN_PARTITION_H
