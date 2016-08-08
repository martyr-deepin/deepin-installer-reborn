// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_SERVICE_PARTITION_MANAGER_STRUCTS_H
#define DEEPIN_INSTALLER_REBORN_SERVICE_PARTITION_MANAGER_STRUCTS_H

#include <QList>
#include <QString>

namespace service {

enum class PartitionTableType {
  Empty,  // Raw disk has empty partition table type.
  GPT,
  MsDos,
  Others,  // Not supported partition types.
  Unknown,
};

enum class FsType {
  Empty,
  Btrfs,
  EFI,
  Ext2,
  Ext3,
  Ext4,
  Fat16,
  Fat32,
  Hfs,
  HfsPlus,
  Jfs,
  LinuxSwap,
  LVM2PV,
  NTFS,
  Others,
  Reiser4,
  Reiserfs,
  Xfs,
  Unknown,
};

struct Partition {
  // File system
  FsType fs = FsType::Unknown;
  qint64 freespace = 0;
  qint64 length = 0;
  QString label;
  QString uuid;
  QStringList mounts;

  // Partition
  QString path;
  qint64 first_sector = 0;
  qint64 last_sector = 0;
  qint64 total_sectors = 0;
  QStringList flags;
  QString os;
};

struct Device {
  QString model;  // Human readable device name (manufacture).
  QString path;
  PartitionTableType table = PartitionTableType::Unknown;
  qint64 freespace = 0;
  qint64 length = 0;  // total sectors / disk capacity.
  int heads = 0;
  int sectors = 0;
  int cylinders = 0;
  qint64 sector_size = 0;
  QList<Partition> partitions;
};

}  // namespace service

#endif  // DEEPIN_INSTALLER_REBORN_SERVICE_PARTITION_MANAGER_STRUCTS_H
