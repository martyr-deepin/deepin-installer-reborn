// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/partition_util.h"

#include <math.h>
#include <QDebug>
#include <QFileInfo>

namespace ui {

QString GetPartitionName(const QString& path) {
  return QFileInfo(path).fileName();
}

QString GetPartitionUsage(qint64 freespace, qint64 total) {
  const qint64 used = total - freespace;
  return QString("%1/%2G").arg(ToGigByte(used)).arg(ToGigByte(total));
}

QString GetFsTypeName(partman::FsType fs_type) {
  switch (fs_type) {
    case partman::FsType::Btrfs:
      return "btrfs";
    case partman::FsType::EFI:
      return "efi";
    case partman::FsType::Ext2:
      return "ext2";
    case partman::FsType::Ext3:
      return "ext3";
    case partman::FsType::Ext4:
      return "ext4";
    case partman::FsType::Fat16:
      return "fat16";
    case partman::FsType::Fat32:
      return "fat32";
    case partman::FsType::Hfs:
      return "hfs";
    case partman::FsType::HfsPlus:
      return "hfs+";
    case partman::FsType::Jfs:
      return "jfs";
    case partman::FsType::LinuxSwap:
      return "linux-swap";
    case partman::FsType::LVM2PV:
      return "lvm2pv";
    case partman::FsType::NTFS:
      return "ntfs";
    case partman::FsType::Others:
      return "others";
    case partman::FsType::Reiser4:
      return "reiser4";
    case partman::FsType::Reiserfs:
      return "reiserfs";
    case partman::FsType::Xfs:
      return "xfs";
    case partman::FsType::Unknown:
      return "unknown";
    default:
      return "unknown";
  }
}

QString GetLocalFsTypeName(partman::FsType fs_type) {
  switch (fs_type) {
    case partman::FsType::Btrfs: return "btrfs journaling file system";
    case partman::FsType::EFI: {
      return QObject::tr("EFI");
    }
    case partman::FsType::Empty: {
      return QObject::tr("do not use this partition");
    }
    case partman::FsType::Ext2: return "Ext2 journaling file system";
    case partman::FsType::Ext3: return "Ext3 journaling file system";
    case partman::FsType::Ext4: return "Ext4 journaling file system";
    case partman::FsType::Fat16: return "FAT16 file system";
    case partman::FsType::Fat32: return "FAT32 file system";
    case partman::FsType::Jfs: return "JFS journaling file system";
    case partman::FsType::LinuxSwap: {
      return QObject::tr("swap area");
    }
    case partman::FsType::Xfs: return "XFS journaling file system";
    default: {
      return QObject::tr("Unknown");
    }
  }
}

bool SupportMountPoint(partman::FsType fs_type) {
  return (fs_type != partman::FsType::EFI &&
          fs_type != partman::FsType::LinuxSwap &&
          fs_type != partman::FsType::Empty &&
          fs_type != partman::FsType::Unknown);
}

int ToGigByte(qint64 size) {
  const double m_size = static_cast<double>(size) / 1024.0;
  qDebug() << size << m_size << (m_size / 1024.0) << (m_size / 1024.0 / 1024.0);
  return  static_cast<int>(round(m_size / 1024.0 / 1024.0));
}


}  // namespace ui