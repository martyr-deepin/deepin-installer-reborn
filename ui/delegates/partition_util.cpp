// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/partition_util.h"

#include <math.h>
#include <QDebug>
#include <QFileInfo>

#include "partman/structs.h"

namespace installer {

QString GetPartitionName(const QString& path) {
  return QFileInfo(path).fileName();
}

QString GetPartitionLabelAndPath(const Partition& partition) {
  // TODO(xushaohua): Trim text to appropriate length.
  const QString name = GetPartitionName(partition.path);
  if (partition.label.isEmpty()) {
    return name;
  } else {
    return QString("%1(%2)").arg(partition.label).arg(name);
  }
}

QString GetPartitionUsage(qint64 freespace, qint64 total) {
  const qint64 used = total - freespace;
  return QString("%1/%2G").arg(ToGigByte(used)).arg(ToGigByte(total));
}

QString GetPartitionUsage(const Partition& partition) {
  if (partition.type == PartitionType::Normal ||
      partition.type == PartitionType::Logical) {
    const qint64 length = (partition.length > 0) ? partition.length :
                          partition.getByteLength();
    return GetPartitionUsage(partition.freespace, length);
  } else {
    const qint64 length = partition.getByteLength();
    return GetPartitionUsage(length, length);
  }
}

int GetPartitionUsageValue(const Partition& partition) {
  if (partition.type == PartitionType::Normal ||
      partition.type == PartitionType::Logical) {
    const qint64 length = (partition.length > 0) ? partition.length :
                          partition.getByteLength();
    return int(100 * (length - partition.freespace) / length);
  } else {
    return 0;
  }
}

QString GetOsTypeIcon(OsType os_type) {
  switch (os_type) {
    case OsType::Empty: {
      return QStringLiteral(":/images/driver_128.png");
    }
    case OsType::Linux: {
      return QStringLiteral(":/images/driver_linux_32.png");
    }
    case OsType::Mac: {
      return QStringLiteral(":/images/driver_mac_32.png");
    }
    case OsType::Unknown: {
      return QStringLiteral(":/images/driver_32.png");
    }
    case OsType::Windows: {
      return QStringLiteral(":/images/driver_windows_32.png");
    }
    default: {
      return QString();
    }
  }
}

QString GetOsTypeLargeIcon(OsType os_type) {
  switch (os_type) {
    case OsType::Empty: {
      return QStringLiteral(":/images/driver_128.png");
    }
    case OsType::Linux: {
      return QStringLiteral(":/images/driver_linux_128.png");
    }
    case OsType::Mac: {
      return QStringLiteral(":/images/driver_mac_128.png");
    }
    case OsType::Unknown: {
      return QStringLiteral(":/images/driver_128.png");
    }
    case OsType::Windows: {
      return QStringLiteral(":/images/driver_windows_128.png");
    }
    default: {
      return QString();
    }
  }
}

QString GetLocalFsTypeName(FsType fs_type) {
  switch (fs_type) {
    case FsType::Btrfs: return "btrfs journaling file system";
    case FsType::EFI: {
      return QObject::tr("EFI");
    }
    case FsType::Empty: {
      return QObject::tr("do not use this partition");
    }
    case FsType::Ext2: return "Ext2 journaling file system";
    case FsType::Ext3: return "Ext3 journaling file system";
    case FsType::Ext4: return "Ext4 journaling file system";
    case FsType::Fat16: return "FAT16 file system";
    case FsType::Fat32: return "FAT32 file system";
    case FsType::Jfs: return "JFS journaling file system";
    case FsType::LinuxSwap: {
      return QObject::tr("swap area");
    }
    case FsType::Xfs: return "XFS journaling file system";
    default: {
      return QObject::tr("Unknown");
    }
  }
}

bool SupportMountPoint(FsType fs_type) {
  return (fs_type != FsType::EFI &&
          fs_type != FsType::LinuxSwap &&
          fs_type != FsType::Empty &&
          fs_type != FsType::Unknown);
}

int ToGigByte(qint64 size) {
  const double m_size = static_cast<double>(size) / kKibiByte;
  return  static_cast<int>(round(m_size / kMebiByte));
}


}  // namespace installer