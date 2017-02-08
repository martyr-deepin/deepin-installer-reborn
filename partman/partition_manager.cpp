// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partition_manager.h"

#include <parted/parted.h>
#include <QDebug>
#include <QDir>
#include <QFile>

#include "base/command.h"
#include "partman/libparted_util.h"
#include "partman/os_prober.h"
#include "partman/partition_usage.h"
#include "partman/utils.h"
#include "sysinfo/dev_disk.h"
#include "sysinfo/proc_mounts.h"

namespace installer {

namespace {

// Absolute path to hook_manager.sh
const char kHookManagerFile[] = BUILTIN_HOOKS_DIR "/hook_manager.sh";

// Get flags of |lp_partition|.
PartitionFlags GetPartitionFlags(PedPartition* lp_partition) {
  Q_ASSERT(lp_partition);
  PartitionFlags flags;
  for (PedPartitionFlag lp_flag =
         ped_partition_flag_next(static_cast<PedPartitionFlag>(NULL));
       lp_flag;
       lp_flag = ped_partition_flag_next(lp_flag)) {
    if (ped_partition_is_flag_available(lp_partition, lp_flag) &&
        ped_partition_get_flag(lp_partition, lp_flag)) {
      flags.append(static_cast<PartitionFlag>(lp_flag));
    }
  }
  return flags;
}

// Read all partitions of |lp_disk|.
PartitionList ReadPartitions(PedDisk* lp_disk) {
  Q_ASSERT(lp_disk);
  PartitionList partitions;
  for (PedPartition* lp_partition = ped_disk_next_partition(lp_disk, nullptr);
      lp_partition != nullptr;
      lp_partition = ped_disk_next_partition(lp_disk, lp_partition)) {

    Partition partition;
    if (lp_partition->type == PED_PARTITION_NORMAL) {
      partition.type = PartitionType::Normal;
    } else if (lp_partition->type == PED_PARTITION_EXTENDED) {
      partition.type = PartitionType::Extended;
    } else if (lp_partition->type ==
        (PED_PARTITION_FREESPACE | PED_PARTITION_LOGICAL)) {
      partition.type = PartitionType::Unallocated;
    } else if (lp_partition->type == PED_PARTITION_LOGICAL) {
      partition.type = PartitionType::Logical;
    } else if (lp_partition->type == PED_PARTITION_FREESPACE) {
      partition.type = PartitionType::Unallocated;
    } else {
      // Ignore other types
      continue;
    }

    // Get partition flags when it is active.
    if (ped_partition_is_active(lp_partition)) {
      partition.flags = GetPartitionFlags(lp_partition);
    }

    if (lp_partition->fs_type) {
      partition.fs = GetFsTypeByName(lp_partition->fs_type->name);
      // If ESP/Boot flag is set on fat16/fat32 partition,
      // it shall be an EFI partition.
      if ((partition.fs == FsType::Fat32 || partition.fs == FsType::Fat16) &&
          partition.flags.contains(PartitionFlag::ESP)) {
        partition.fs = FsType::EFI;
      }
    } else {
      partition.fs = FsType::Unknown;
    }
    partition.start_sector = lp_partition->geom.start;
    partition.end_sector = lp_partition->geom.end;

    partition.partition_number = lp_partition->num;
    partition.path = GetPartitionPath(lp_partition);

    // Avoid reading additional filesystem information if there is no path.
    if (!partition.path.isEmpty() &&
        partition.type != PartitionType::Unallocated &&
        partition.type != PartitionType::Extended) {

      // Read label based on filesystem type
      ReadUsage(partition.path, partition.fs, partition.freespace,
                partition.length);
      // If LinuxSwap partition is not mount, it is totally free.
      if (partition.fs == FsType::LinuxSwap && partition.length <= 0) {
        partition.length = partition.getByteLength();
        partition.freespace = partition.length;
      }

      // Get partition name.
      partition.name = ped_partition_get_name(lp_partition);
    }
    partitions.append(partition);
  }
  return partitions;
}

// Unmount devices and swap partitions.
bool UnmountDevices() {
  // Swap off partitions and files.
  bool ok;
  QString out, err;
  ok = SpawnCmd("swapoff", {"--all"}, out, err);
  if (!ok) {
    qWarning() << "swapoff failed!" << out << err;
  }
  const char kTargetDir[] = "/target";
  if (QDir(kTargetDir).exists()) {
    if (!SpawnCmd("umount", {"-R", kTargetDir}, out, err)) {
      ok = false;
      qWarning() << "umount /target failed" << out << err;
    }
  }

  // TODO(xushaohua): Handles lvm.

  // umount --all almost always returns false.
  //SpawnCmd("umount", {"--all"}, out, err);

  return ok;
}

}  // namespace

PartitionManager::PartitionManager(QObject* parent) : QObject(parent) {
  this->setObjectName("partition_manager");

  qRegisterMetaType<DeviceList>("DeviceList");
  qRegisterMetaType<OperationList>("OperationList");
  this->initConnections();
}

PartitionManager::~PartitionManager() {
  // No need to release objects in operation list.
  // It is released in PartitionDelegate.
}

void PartitionManager::initConnections() {
  connect(this, &PartitionManager::refreshDevices,
          this, &PartitionManager::doRefreshDevices);
  connect(this, &PartitionManager::autoPart,
          this, &PartitionManager::doAutoPart);
  connect(this, &PartitionManager::manualPart,
          this, &PartitionManager::doManualPart);
}

void PartitionManager::doRefreshDevices(bool umount, bool enable_os_prober) {
  // Umount devices first.
  if (umount) {
    UnmountDevices();
  }

  const DeviceList devices = ScanDevices(enable_os_prober);
  emit this->devicesRefreshed(devices);
}

void PartitionManager::doAutoPart(const QString& script_path) {
  if (!QFile::exists(script_path)) {
    qCritical() << "partition script file not found!" << script_path;
    emit this->autoPartDone(false);
    return;
  }
  QString output, err;
  const bool ok = RunScriptFile({kHookManagerFile, script_path}, output, err);
  if (!ok) {
    qCritical() << "Output of auto_part:" << output
                << "Error of auto_part:" << err;
  }
  emit this->autoPartDone(ok);
}

void PartitionManager::doManualPart(const OperationList& operations) {
  qDebug() << "operations:" << operations;
  bool ok = true;
  for (int i = 0; ok && i < operations.length(); ++i) {
    ok = operations.at(i).applyToDisk();
    qDebug() << "operation result:" << ok;
  }

  emit this->manualPartDone(ok);
}

DeviceList ScanDevices(bool enable_os_prober) {
  // 1. List Devices
  // 1.1. Retrieve metadata of each device.
  // 2. List partitions of each device.
  // 3. Retrieve partition metadata.

  // Let libparted detect all devices and construct device list.
  ped_device_probe_all();

  DeviceList devices;
  const LabelItems label_items = ParseLabelDir();
  const MountItemList mount_items = ParseMountItems();

  OsTypeItems os_type_items;
  if (enable_os_prober) {
    os_type_items = GetOsTypeItems();
  }

  for (PedDevice* lp_device = ped_device_get_next(nullptr);
      lp_device != nullptr;
      lp_device = ped_device_get_next(lp_device)) {
    PedDiskType* disk_type = ped_disk_probe(lp_device);
    PedDisk* lp_disk = nullptr;
    // TODO(xushaohua): Filters USB device.
    if (disk_type == nullptr) {
      // Raw disk found.
      if (IsEfiEnabled()) {
        disk_type = ped_disk_type_get(kPartitionTableGPT);
      } else {
        disk_type = ped_disk_type_get(kPartitionTableMsDos);
      }
      if (disk_type != NULL) {
        // Create a new device table but not commit changes to device.
        lp_disk = ped_disk_new_fresh(lp_device, disk_type);
        if (lp_disk) {
          // Commit explicitly. Or else newly created partition table is only
          // in memory.
          // TODO(xushaohua): Add FormatDisk in operation.h
          Commit(lp_disk);
        }
      } else {
        qCritical() << "ScanDevices() disk_type is nullptr";
      }
    } else if (QString(kPartitionTableGPT) == disk_type->name ||
               QString(kPartitionTableMsDos) == disk_type->name) {
      lp_disk = ped_disk_new(lp_device);
    } else {
      // Ignores other type of device.
      qWarning() << "Ignores other type of device:" << lp_device->path
                 << disk_type->name;
      continue;
    }

    Device device;
    device.path = lp_device->path;
    device.model = lp_device->model;
    device.length = lp_device->length;
    device.sector_size = lp_device->sector_size;
    device.heads = lp_device->bios_geom.heads;
    device.sectors = lp_device->bios_geom.sectors;
    device.cylinders = lp_device->bios_geom.cylinders;
    const QString disk_type_name(disk_type->name);
    if (disk_type_name == kPartitionTableMsDos) {
      device.table = PartitionTableType::MsDos;
    } else if (disk_type_name == kPartitionTableGPT) {
      device.table = PartitionTableType::GPT;
    }
    device.max_prims = ped_disk_get_max_primary_partition_count(lp_disk);

    device.partitions = ReadPartitions(lp_disk);
    // Add additional info to partitions.
    for (Partition& partition : device.partitions) {
      partition.device_path = device.path;
      partition.sector_size = device.sector_size;
      if (!partition.path.isEmpty() &&
          partition.type != PartitionType::Unallocated) {
        // Read partition label and os.
        const QString empty_str;
        partition.label = label_items.value(partition.path, empty_str);
        partition.os = os_type_items.value(partition.path, OsType::Empty);

        // Mark busy flag of this partition when it is mounted in system.
        for (const MountItem& mount_item : mount_items) {
          if (mount_item.path == partition.path) {
            partition.busy = true;
            break;
          }
        }
      }
    }

    devices.append(device);
    ped_disk_destroy(lp_disk);
  }

  return devices;
}

}  // namespace installer
