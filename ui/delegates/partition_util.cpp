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

void AlignPartition(Partition& partition) {
  const qint64 oneMebiByteSector = 1 * kMebiByte / partition.sector_size;

  // Align to nearest MebiBytes.
  const int start_size = static_cast<int>(
      ceil(partition.start_sector * 1.0 / oneMebiByteSector));
  const int end_size = static_cast<int>(
      floor((partition.end_sector + 1) * 1.0 / oneMebiByteSector));
  partition.start_sector = start_size * oneMebiByteSector;
  partition.end_sector = end_size * oneMebiByteSector - 1;
}

int AllocLogicalPartitionNumber(const Device& device) {
  int num = device.max_prims;
  for (const Partition& partition : device.partitions) {
    if (partition.partition_number >= num) {
      num = partition.partition_number;
    }
  }
  return num + 1;
}

int AllocPrimaryPartitionNumber(const Device& device) {
  for (int num = 1; num <= device.max_prims; num++) {
    bool in_use = false;
    for (const Partition& partition : device.partitions) {
      if (partition.partition_number == num) {
        in_use = true;
        break;
      }
    }

    if (!in_use) {
      return num;
    }
  }
  return -1;
}

DeviceList FilterInstallerDevice(const DeviceList& devices) {
  DeviceList filtered_devices;
  const QString installer_device_path(GetInstallerDevicePath());
  for (const Device& device : devices) {
    if (!installer_device_path.startsWith(device.path)) {
      filtered_devices.append(device);
    }
  }

  return filtered_devices;
}

FsType GetDefaultFsType() {
  const QString default_fs_name = GetSettingsString(kPartitionDefaultFs);
  return GetFsTypeByName(default_fs_name);
}

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
    case FsType::EFI: return "efi";
    case FsType::Empty: return QObject::tr("Do not use this partition");
    case FsType::Ext2: return "ext2";
    case FsType::Ext3: return "ext3";
    case FsType::Ext4: return "ext4";
    case FsType::Fat16: return "fat16";
    case FsType::Fat32: return "fat32";
    case FsType::Jfs: return "jfs";
    case FsType::LinuxSwap: return QObject::tr("Swap partition");
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

bool IsEfiEnabled() {
  return QDir("/sys/firmware/efi").exists();
}

// Returns true if |fs_type| may be mounted to system with customized
// mount point.
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

bool IsPartitionTableMatch(const DeviceList& devices,
                           const QString& device_path) {
  const int device_index = DeviceIndex(devices, device_path);
  if (device_index == -1) {
    qCritical() << "Failed to find device:" << device_path;
    return false;
  }
  PartitionTableType table = devices.at(device_index).table;
  return IsPartitionTableMatch(table);
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