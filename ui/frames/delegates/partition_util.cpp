// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partition_util.h"

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
    case partman::FsType::Btrfs: return "btrfs";
    case partman::FsType::EFI: {
      return QObject::tr("EFI");
    }
    case partman::FsType::Ext2: return "Ext2";
    case partman::FsType::Ext3: return "Ext3";
    case partman::FsType::Ext4: return "Ext4";
    case partman::FsType::Fat16: return "FAT16";
    case partman::FsType::Fat32: return "AFAT32";
    case partman::FsType::Hfs: return "Hfs";
    case partman::FsType::HfsPlus: return "Hfs Plus";
    case partman::FsType::Jfs: return "Jfs";
    case partman::FsType::LinuxSwap: {
      return QObject::tr("Linux Swap");
    }
    case partman::FsType::LVM2PV: {
      return QObject::tr("LVM2 Physical Volume");
    }
    case partman::FsType::NTFS: return "NTFS";
    case partman::FsType::Others: return "Others";
    case partman::FsType::Reiser4: return "Reiser4";
    case partman::FsType::Reiserfs: return "Reiserfs";
    case partman::FsType::Xfs: return "Xfs";
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