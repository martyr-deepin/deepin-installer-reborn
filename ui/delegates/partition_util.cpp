// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/partition_util.h"

#include <math.h>
#include <QDebug>
#include <QFileInfo>

#include "partman/structs.h"

namespace installer {

QString GetDeviceModelAndCap(const Device& device) {
  const int gibi_size = ToGigByte(device.getByteLength());
  return QString("%1 (%2G)").arg(device.model).arg(gibi_size);
}

QString GetPartitionName(const QString& path) {
  return QFileInfo(path).fileName();
}

QString GetPartitionLabel(const Partition& partition) {
  switch (partition.type) {
    case PartitionType::Unallocated: {
      return QObject::tr("Freespace");
    }
    case PartitionType::Normal:  // pass through
    case PartitionType::Logical: {
      if (partition.label.isEmpty()) {
        return GetPartitionName(partition.path);
      } else {
        // TODO(xushaohua): trim text
        return partition.label;
      }
    }
    default: {
      return QString();
    }
  }
}

QString GetPartitionLabelAndPath(const Partition& partition) {
  switch (partition.type) {
    case PartitionType::Unallocated: {
      return QObject::tr("Freespace");
    }
    case PartitionType::Normal:  // pass through
    case PartitionType::Logical: {
      // TODO(xushaohua): Trim text to appropriate length.
      const QString name = GetPartitionName(partition.path);
      if (partition.label.isEmpty()) {
        return name;
      } else {
        return QString("%1(%2)").arg(partition.label).arg(name);
      }
    }
    default: {
      return QString();
    }
  }
}

QString GetPartitionUsage(const Partition& partition) {
  qint64 total, used;
  if (partition.type == PartitionType::Normal ||
      partition.type == PartitionType::Logical) {
    total = (partition.length > 0) ? partition.length :
             partition.getByteLength();
    used = total - partition.freespace;
  } else {
    total = partition.getByteLength();
    used = 0;
  }

  if (total < 1 * kGibiByte) {
    return QString("%1/%2M").arg(ToMebiByte(used)).arg(ToMebiByte(total));
  } else {
    return QString("%1/%2G").arg(ToGigByte(used)).arg(ToGigByte(total));
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
    case OsType::Linux: {
      return ":/images/driver_linux_32.png";
    }
    case OsType::Mac: {
      return ":/images/driver_mac_32.png";
    }
    case OsType::Windows: {
      return ":/images/driver_windows_32.png";
    }
    default: {
      return ":/images/driver_32.png";
    }
  }
}

QString GetOsTypeLargeIcon(OsType os_type) {
  switch (os_type) {
    case OsType::Linux: {
      return ":/images/driver_linux_128.png";
    }
    case OsType::Mac: {
      return ":/images/driver_mac_128.png";
    }
    case OsType::Windows: {
      return ":/images/driver_windows_128.png";
    }
    default: {
      return ":/images/driver_128.png";
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
  const double m_size = double(size) / kKibiByte;
  return int(round(m_size / kMebiByte));
}

int ToMebiByte(qint64 size) {
  return int(round(double(size) / kMebiByte));
}

}  // namespace installer