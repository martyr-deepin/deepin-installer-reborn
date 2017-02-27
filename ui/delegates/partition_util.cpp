// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/partition_util.h"

#include <math.h>
#include <QFileInfo>

#include "partman/utils.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "sysinfo/proc_meminfo.h"
#include "sysinfo/proc_mounts.h"

namespace installer {

namespace {

// Maximum length of partition label.
const int kLabelMaxLen = 12;

// Mount points of live system in use currently.
const char kCasperMountPoint[] = "/cdrom";
const char kLiveMountPoint[] = "/lib/live/mount/medium";

}  // namespace

QString GetDeviceModelAndCap(const Device& device) {
  const int gibi_size = ToGigByte(device.getByteLength());
  return QString("%1 (%2G)").arg(device.model).arg(gibi_size);
}

QString GetDeviceModelCapAndPath(const Device& device) {
  const int gibi_size = ToGigByte(device.getByteLength());
  const QString name(QFileInfo(device.path).fileName());
  return QString("%1 %2G(%3)").arg(device.model).arg(gibi_size).arg(name);
}

QString GetInstallerDevicePath() {
  const MountItemList list(ParseMountItems());

  for (const MountItem& item : list) {
    if (item.mount == kCasperMountPoint || item.mount == kLiveMountPoint) {
      return item.path;
    }
  }

  // Returns an empty string if not found.
  return QString();
}

QString GetLocalFsTypeName(FsType fs_type) {
  switch (fs_type) {
    case FsType::Btrfs: return "btrfs";
    case FsType::EFI: return QObject::tr("efi");
    case FsType::Empty: return QObject::tr("do not use this partition");
    case FsType::Ext2: return "ext2";
    case FsType::Ext3: return "ext3";
    case FsType::Ext4: return "ext4";
    case FsType::Fat16: return "fat16";
    case FsType::Fat32: return "fat32";
    case FsType::Jfs: return "jfs";
    case FsType::LinuxSwap: return QObject::tr("swap area");
    case FsType::Xfs: return "xfs";
    default: return QObject::tr("Unknown");
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

QString GetPartitionLabel(const Partition& partition) {
  switch (partition.type) {
    case PartitionType::Unallocated: {
      return QObject::tr("Freespace");
    }
    case PartitionType::Normal:  // pass through
    case PartitionType::Logical: {
      if (!partition.name.isEmpty()) {
        return TrimText(partition.name, kLabelMaxLen);
      } else if (!partition.label.isEmpty()) {
        return TrimText(partition.label, kLabelMaxLen);
      } else {
        return GetPartitionName(partition.path);
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
      const QString name = GetPartitionName(partition.path);
      if (!partition.name.isEmpty()) {
        const QString label = TrimText(partition.name, kLabelMaxLen);
        return QString("%1(%2)").arg(label).arg(name);
      } else if (!partition.label.isEmpty()) {
        const QString label = TrimText(partition.label, kLabelMaxLen);
        return QString("%1(%2)").arg(label).arg(name);
      } else {
        return name;
      }
    }
    default: {
      return QString();
    }
  }
}

QString GetPartitionName(const QString& path) {
  return QFileInfo(path).fileName();
}

QString GetPartitionUsage(const Partition& partition) {
  qint64 total, used;
  if ((partition.length > 0) && (partition.length >= partition.freespace)) {
    total = partition.length;
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
  qint64 total, used;
  if ((partition.length > 0) && (partition.length >= partition.freespace)) {
    total = partition.length;
    used = total - partition.freespace;
  } else {
    total = partition.getByteLength();
    used = 0;
  }
  if (total > 0 && used >= 0 && total >= used) {
    return int(100.0 * used / total);
  } else {
    return 0;
  }
}

bool IsMountPointSupported(FsType fs_type) {
  return (fs_type != FsType::EFI &&
          fs_type != FsType::LinuxSwap &&
          fs_type != FsType::Empty &&
          fs_type != FsType::Unknown);
}

bool IsPartitionTableMatch(PartitionTableType type) {
  // If EFI is not enabled, always returns true.
  if (!IsEfiEnabled()) {
    return true;
  }

  // If partition table is empty(a raw disk device), returns true.
  if (type == PartitionTableType::Empty) {
    return true;
  }

  return type == PartitionTableType::GPT;
}

bool IsSwapAreaNeeded() {
  const MemInfo mem_info = GetMemInfo();
  const qint64 mem_threshold =
      GetSettingsInt(kPartitionMemoryThresholdForSwapArea) * kGibiByte;
  return mem_info.mem_total <= mem_threshold;
}

int ToGigByte(qint64 size) {
  const double m_size = double(size) / kKibiByte;
  return int(round(m_size / kMebiByte));
}

int ToMebiByte(qint64 size) {
  return int(round(double(size) / kMebiByte));
}

QString TrimText(const QString& text, int max_len) {
  if (text.length() > max_len) {
    return text.left(max_len) + "..";
  } else {
    return text;
  }
}

}  // namespace installer