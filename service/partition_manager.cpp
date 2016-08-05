// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "service/partition_manager.h"

#include <parted/parted.h>
#include <string.h>
#include <QDebug>
#include <QDir>

#include "base/command.h"
#include "service/settings_manager.h"
#include "service/inner/partition_format.h"
#include "service/inner/partition_label.h"
#include "service/inner/partition_usage.h"
#include "sysinfo/dev_uuid.h"
#include "sysinfo/proc_partitions.h"

namespace service {

namespace {

const char kPartationTableGPT[] = "gpt";
const char kPartationTableMsDos[] = "msdos";

//// Flush linux kernel caches. Ensure coherency between the caches of the whole
//// disk device and the partition devices.
//bool FlushDevice(PedDevice* device) {
//  bool success = false;
//  if (ped_device_open(device)) {
//    success = static_cast<bool>(ped_device_sync(device));
//    ped_device_close(device);
//  }
//  return success;
//}
//
//// Refers: http://stackoverflow.com/questions/14127210/
//// After the kernel boots, `udevd` is used to create device nodes for
//// all detected devices. That is a relatively time consuming task that
//// has to be completed for the boot process to continue, otherwise
//// there is a risk of services failing due to missing device nodes.
//// `udevadm settle` waits for udevd to process the device creation events
//// for all hardware devices, thus ensuring that any device nodes
//// have been created successfully before proceeding.
//void SettleDevice(int timeout) {
//  const QString cmd = QString("udevadm settle --timeout=%1").arg(timeout);
//  // TODO(xushaohua):
//  Q_UNUSED(cmd);
//}
//
//bool CommitDiskChanges(PedDisk* disk) {
//  bool success = static_cast<bool>(ped_disk_commit_to_dev(disk));
//  if (success) {
//    success = static_cast<bool>(ped_disk_commit_to_os(disk));
//  }
//  return success;
//}

// Make filesystem on |partition| based on its fs type.
void Mkfs(Partition& partition) {
  switch (partition.fs) {
    case FsType::Btrfs: {
      FormatBtrfs(partition.path, partition.label);
      break;
    }
    case FsType::Ext2: {
      FormatExt2(partition.path, partition.label);
      break;
    }
    case FsType::Ext3: {
      FormatExt3(partition.path, partition.label);
      break;
    }
    case FsType::Ext4: {
      FormatExt4(partition.path, partition.label);
      break;
    }
    case FsType::Fat16: {
      FormatFat16(partition.path, partition.label);
      break;
    }
    case FsType::EFI:
    case FsType::Fat32: {
      FormatFat32(partition.path, partition.label);
      break;
    }
    case FsType::Hfs: {
      FormatHfs(partition.path, partition.label);
      break;
    }
    case FsType::HfsPlus: {
      FormatHfsPlus(partition.path, partition.label);
      break;
    }
    case FsType::Jfs: {
      FormatJfs(partition.path, partition.label);
      break;
    }
    case FsType::LinuxSwap: {
      FormatLinuxSwap(partition.path, partition.label);
      break;
    }
    case FsType::NTFS: {
      FormatNTFS(partition.path, partition.label);
      break;
    }
    case FsType::Reiser4: {
      FormatReiser4(partition.path, partition.label);
      break;
    }
    case FsType::Reiserfs: {
      FormatReiserfs(partition.path, partition.label);
      break;
    }
    case FsType::Xfs: {
      FormatXfs(partition.path, partition.label);
      break;
    }
    default: {
      qWarning() << "Unsupported filesystem to format:"
                 << GetFsTypeName(partition.fs);
    }
  }
}

// Update partition label.
void ReadLabel(Partition& partition) {
  switch (partition.fs) {
    case FsType::Btrfs: {
      partition.label = ReadBtrfsLabel(partition.path);
      break;
    }
    case FsType::Ext2:
    case FsType::Ext3:
    case FsType::Ext4: {
      partition.label = ReadExt2Label(partition.path);
      break;
    }
    case FsType::EFI:
    case FsType::Fat16:
    case FsType::Fat32: {
      partition.label = ReadFat16Label(partition.path);
      break;
    }
    case FsType::Hfs:
    case FsType::HfsPlus: {
      partition.label = ReadHfsLabel(partition.path);
      break;
    }
    case FsType::Jfs: {
      partition.label = ReadJfsLabel(partition.path);
      break;
    }
    case FsType::LinuxSwap: {
      partition.label = ReadLinuxSwapLabel(partition.path);
      break;
    }
    case FsType::NTFS: {
      partition.label = ReadNTFSLabel(partition.path);
      break;
    }
    case FsType::Reiser4: {
      partition.label = ReadReiser4Label(partition.path);
      break;
    }
    case FsType::Reiserfs: {
      partition.label = ReadReiserfsLabel(partition.path);
      break;
    }
    case FsType::Xfs: {
      partition.label = ReadXfsLabel(partition.path);
      break;
    }
    default: {
      partition.label = "";
      break;
    }
  }
}

// Update partition usage.
void ReadUsage(Partition& partition) {
  bool ok = false;
  qint64 total = 0;
  qint64 freespace = 0;
  switch (partition.fs) {
    case FsType::Btrfs: {
      ok = ReadBtrfsUsage(partition.path, freespace, total);
      break;
    }
    case FsType::Ext2:
    case FsType::Ext3:
    case FsType::Ext4: {
      ok = ReadExt2Usage(partition.path, freespace, total);
      break;
    }
    case FsType::EFI:
    case FsType::Fat16:
    case FsType::Fat32: {
      ok = ReadFat16Usage(partition.path, freespace, total);
      break;
    }
    case FsType::Hfs:
    case FsType::HfsPlus: {
      break;
    }
    case FsType::Jfs: {
      ok = ReadJfsUsage(partition.path, freespace, total);
      break;
    }
    case FsType::LinuxSwap: {
      ok = ReadLinuxSwapUsage(partition.path, freespace, total);
      break;
    }
    case FsType::NTFS: {
      ok = ReadNTFSUsage(partition.path, freespace, total);
      break;
    }
    case FsType::Reiser4: {
      ok = ReadReiser4Usage(partition.path, freespace, total);
      break;
    }
    case FsType::Reiserfs: {
      ok = ReadReiserfsUsage(partition.path, freespace, total);
      break;
    }
    case FsType::Xfs: {
      ok = ReadXfsUsage(partition.path, freespace, total);
      break;
    }
    default: {
      break;
    }
  }
  if (ok) {
    partition.length = total;
    partition.freespace = freespace;
  } else {
    partition.length = 0;
    partition.freespace = 0;
  }
}

}  // namespace

QString GetFsTypeName(FsType fs_type) {
  switch (fs_type) {
    case FsType::Btrfs: return "btrfs";
    case FsType::EFI: return "efi";
    case FsType::Ext2: return "ext2";
    case FsType::Ext3: return "ext3";
    case FsType::Ext4: return "ext4";
    case FsType::Fat16: return "fat16";
    case FsType::Fat32: return "fat32";
    case FsType::Hfs: return "hfs";
    case FsType::HfsPlus: return "hfs+";
    case FsType::Jfs: return "jfs";
    case FsType::LinuxSwap: return "linux-swap";
    case FsType::LVM2PV: return "lvm2pv";
    case FsType::NTFS: return "ntfs";
    case FsType::Others: return "others";
    case FsType::Reiser4: return "reiser4";
    case FsType::Reiserfs: return "reiserfs";
    case FsType::Xfs: return "xfs";
    default: return "unknown";
  }
}

FsType GetFsTypeByName(const QString& name) {
  const QString lower = name.toLower();
  if (lower == "") return FsType::Empty;
  if (lower == "btrfs") return FsType::Btrfs;
  if (lower == "efi") return FsType::EFI;
  if (lower == "ext2") return FsType::Ext2;
  if (lower == "ext3") return FsType::Ext3;
  if (lower == "ext4") return FsType::Ext4;
  if (lower == "fat16") return FsType::Fat16;
  if (lower == "fat32") return FsType::Fat32;
  if (lower == "hfs") return FsType::Hfs;
  if (lower == "hfs+") return FsType::HfsPlus;
  if (lower == "jfs") return FsType::Jfs;
  if (lower == "linux-swap") return FsType::LinuxSwap;
  if (lower == "lvm2pv") return FsType::LVM2PV;
  if (lower == "ntfs") return FsType::NTFS;
  if (lower == "others") return FsType::Others;
  if (lower == "reiser4") return FsType::Reiser4;
  if (lower == "resierfs") return FsType::Reiserfs;
  if (lower == "xfs") return FsType::Xfs;
  return FsType::Unknown;
}

PartitionManager::PartitionManager(QObject* parent) : QObject(parent) {
  this->setObjectName(QStringLiteral("partition_manager"));

  this->initConnections();
}

void PartitionManager::initConnections() {
  connect(this, &PartitionManager::refreshDevices,
          this, &PartitionManager::doRefreshDevices);
  connect(this, &PartitionManager::autoPart,
          this, &PartitionManager::doAutoPart);
  connect(this, &PartitionManager::manualPart,
          this, &PartitionManager::doManualPart);
}

void PartitionManager::doRefreshDevices() {
// 1. List Devices
// 1.1. Retrieve metadata of each device.
//   * partition-table type
//   * label
//   * path
//   * capacity
//   * freespace
// 2. List partitions of each device.
// 3. Retrieve partition metadata.
//   * partition label.
//   * path
//   * uuid
//   * os-probe
//   * filesystem type
//   * freespace
//   * total capacity
//   * mounted ?
//   * mount paths

  // Let libparted detect all devices and construct device list.
  ped_device_probe_all();

  QList<Device> devices;
  const sysinfo::UUIDItems uuid_items = sysinfo::ParseUUIDDir();

  PedDevice* p_device = NULL;
  while ((p_device = ped_device_get_next(p_device)) != NULL) {
    PedDiskType* device_type = ped_disk_probe(p_device);
    PedDisk* disk = NULL;
    // TODO(xushaohua): Filters USB device.
    if (device_type == NULL) {
      // Raw disk found.
      // TODO(xushaohua): Check proper partition type.
      disk = ped_disk_new_fresh(p_device,
                                ped_disk_type_get(kPartationTableGPT));
      device_type = ped_disk_probe(p_device);
    } else if (QString(kPartationTableGPT) == device_type->name ||
               QString(kPartationTableMsDos) == device_type->name) {
      disk = ped_disk_new(p_device);
    } else {
      // Ignores other type of device.
      qWarning() << "Ignores other type of device:" << device_type->name;
      continue;
    }

    Device device;
    device.model = p_device->model;
    device.path = p_device->path;
    if (strncmp(kPartationTableGPT, device_type->name, 3)) {
      device.table = PartitionTableType::GPT;
    } else if (strncmp(kPartationTableMsDos, device_type->name, 5)) {
      device.table = PartitionTableType::MsDos;
    }
    device.heads = p_device->bios_geom.heads;
    device.sectors = p_device->bios_geom.sectors;
    device.cylinders = p_device->bios_geom.cylinders;
    device.sector_size = p_device->sector_size;

    QList<Partition> partitions;
    PedPartition* p_partition = NULL;
    while ((p_partition = ped_disk_next_partition(disk, p_partition)) != NULL) {
      Partition partition;
      partition.fs = GetFsTypeByName(p_partition->fs_type->name);
      ReadLabel(partition);
      partition.first_sector = p_partition->geom.start;
      partition.total_sectors = p_partition->geom.length;
      partition.last_sector = p_partition->geom.end;
      partition.path = ped_partition_get_path(p_partition);
      // Avoid reading additional filesystem information if there is no path.
      if (!partition.path.isEmpty()) {
        // Read uuid from /dev/disk/by-uuid/
        partition.uuid = uuid_items.value(partition.path);

        // Read label based on filesystem type
        ReadUsage(partition);
 
        // Read possible Operating System version by calling `os-prober`.

        partition.name = ped_partition_get_name(p_partition);
      }
      //partition.flags = {};
    }

    device.partitions = partitions;
    devices.append(device);
    //ped_disk_print(disk);
//    ped_device_destroy(p_device);
//    ped_disk_destroy(disk);
  }
}

void PartitionManager::doAutoPart() {
  const QString filepath = GetAutoPartFile();
  if (!QFile::exists(filepath)) {
    qCritical() << "oem/auto-part.js not found!";
  }
  const bool ok = base::RunScriptFile(filepath);
  emit this->autoPartDone(ok);
}

void PartitionManager::doManualPart() {
  bool ok = true;
  // TODO(xushaohua):
  emit this->manualPartDone(ok);
}

bool IsEfiEnabled() {
  return QDir(QStringLiteral("/sys/firmware/efi")).exists();
}

qint64 GetMaximumDeviceSize() {
  const sysinfo::PartitionItemList list = sysinfo::ParsePartitionItems();
  qint64 result = 0;
  for (const sysinfo::PartitionItem item : list) {
    result = qMax(result, item.blocks);
  }
  return result;
}

PartitionTableType GetPrimaryDiskPartitionTable() {
  // TODO(xushaohua): Read path to the first device.
  PedDevice* device = ped_device_get("/dev/sda");
  if (device == NULL) {
    return PartitionTableType::Unknown;
  }
  PedDiskType* disk_type = ped_disk_probe(device);
  if (disk_type == NULL) {
    return PartitionTableType::Empty;
  }
  if (strncmp(kPartationTableGPT, disk_type->name, 3)) {
    return PartitionTableType::GPT;
  }
  if (strncmp(kPartationTableMsDos, disk_type->name, 5)) {
    return PartitionTableType::MsDos;
  }
  return PartitionTableType::Others;
}

}  // namespace service
