/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "partition_manager.h"

#include <parted/parted.h>
#include <QDebug>
#include <QDir>

#include "base/command.h"
#include "partman/libparted_util.h"
#include "partman/os_prober.h"
#include "partman/partition_usage.h"
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

    Partition::Ptr partition (new Partition);
    if (lp_partition->type == PED_PARTITION_NORMAL) {
      partition->type = PartitionType::Normal;
    } else if (lp_partition->type == PED_PARTITION_EXTENDED) {
      partition->type = PartitionType::Extended;
    } else if (lp_partition->type ==
        (PED_PARTITION_FREESPACE | PED_PARTITION_LOGICAL)) {
      partition->type = PartitionType::Unallocated;
    } else if (lp_partition->type == PED_PARTITION_LOGICAL) {
      partition->type = PartitionType::Logical;
    } else if (lp_partition->type == PED_PARTITION_FREESPACE) {
      partition->type = PartitionType::Unallocated;
    } else {
      // Ignore other types
      continue;
    }

    // Get partition flags when it is active.
    if (ped_partition_is_active(lp_partition)) {
      partition->flags = GetPartitionFlags(lp_partition);
    }

    if (lp_partition->fs_type) {
      partition->fs = GetFsTypeByName(lp_partition->fs_type->name);
      // If ESP/Boot flag is set on fat16/fat32 partition,
      // it shall be an EFI partition->
      if ((partition->fs == FsType::Fat32 || partition->fs == FsType::Fat16) &&
          partition->flags.contains(PartitionFlag::ESP)) {
        partition->fs = FsType::EFI;
      }
    } else {
      partition->fs = FsType::Unknown;
    }
    partition->start_sector = lp_partition->geom.start;
    partition->end_sector = lp_partition->geom.end;

    partition->partition_number = lp_partition->num;
    partition->path = GetPartitionPath(lp_partition);

    // Avoid reading additional filesystem information if there is no path.
    if (!partition->path.isEmpty() &&
        partition->type != PartitionType::Unallocated &&
        partition->type != PartitionType::Extended) {

      // Read label based on filesystem type
      ReadUsage(partition->path, partition->fs, partition->freespace,
                partition->length);
      // If LinuxSwap partition is not mount, it is totally free.
      if (partition->fs == FsType::LinuxSwap && partition->length <= 0) {
        partition->length = partition->getByteLength();
        partition->freespace = partition->length;
      }

      // Get partition name.
      partition->name = ped_partition_get_name(lp_partition);
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

PartitionManager::PartitionManager(QObject* parent)
    : QObject(parent),
      enable_os_prober_(true) {
  this->setObjectName("partition_manager");

  // Register meta types used in signals.
  qRegisterMetaType<DeviceList>("DeviceList");
  qRegisterMetaType<OperationList>("OperationList");
  qRegisterMetaType<PartitionTableType>("PartitionTableType");
  this->initConnections();
}

PartitionManager::~PartitionManager() {
  // No need to release objects in operation list.
  // It is released in PartitionDelegate.
}

void PartitionManager::initConnections() {
  connect(this, &PartitionManager::createPartitionTable,
          this, &PartitionManager::doCreatePartitionTable);
  connect(this, &PartitionManager::refreshDevices,
          this, &PartitionManager::doRefreshDevices);
  connect(this, &PartitionManager::autoPart,
          this, &PartitionManager::doAutoPart);
  connect(this, &PartitionManager::manualPart,
          this, &PartitionManager::doManualPart);
}

void PartitionManager::doCreatePartitionTable(const QString& device_path,
                                              PartitionTableType table) {
  if (!CreatePartitionTable(device_path, table)) {
    qCritical() << "PartitionManager failed to create partition table at"
                << device_path;
  }
  const DeviceList devices = ScanDevices(enable_os_prober_);
  emit this->devicesRefreshed(devices);
}

void PartitionManager::doRefreshDevices(bool umount, bool enable_os_prober) {
  // Umount devices first.
  if (umount) {
    UnmountDevices();
  }

  enable_os_prober_ = enable_os_prober;
  const DeviceList devices = ScanDevices(enable_os_prober);
  emit this->devicesRefreshed(devices);
}

void PartitionManager::doAutoPart(const QString& script_path) {
  if (!QFile::exists(script_path)) {
    qCritical() << "partition script file not found!" << script_path;
    emit this->autoPartDone(false);
    return;
  }
  const bool ok = RunScriptFile({kHookManagerFile, script_path});
  emit this->autoPartDone(ok);
}

void PartitionManager::doManualPart(const OperationList& operations) {
  qDebug() << Q_FUNC_INFO << "\n" << "operations:" << operations;
  bool ok = true;
  // Copy operation list, as partition path will be updated in applyToDisk().
  OperationList real_operations(operations);
  for (int i = 0; ok && i < real_operations.length(); ++i) {
    Operation& operation = real_operations[i];
    ok = operation.applyToDisk();
  }
  qDebug() << Q_FUNC_INFO << "\n" << "real operations:" << real_operations;

  DeviceList devices;
  if (ok) {
    devices = ScanDevices(false);
    // Update mount point of real partitions.
    for (Device::Ptr device : devices) {
      for (Partition::Ptr partition : device->partitions) {
        for (const Operation& operation : real_operations) {
          if (operation.type == OperationType::NewPartTable) continue; // skip for create table
          if (operation.new_partition->path == partition->path) {
            partition->mount_point = operation.new_partition->mount_point;
          }
        }
      }
    }
  }

  emit this->manualPartDone(ok, devices);
}

DeviceList ScanDevices(bool enable_os_prober) {
  // 1. List Devices
  // 1.1. Retrieve metadata of each device->
  // 2. List partitions of each device->
  // 3. Retrieve partition metadata.

  // Let libparted detect all devices and construct device list.
  ped_device_probe_all();

  DeviceList devices;
  const LabelItems label_items = ParseLabelDir();
  const MountItemList mount_items = ParseMountItems();

  OsProberItems os_prober_items;
  if (enable_os_prober) {
    os_prober_items = GetOsProberItems();
  }

  // Walk through all devices.
  for (PedDevice* lp_device = ped_device_get_next(nullptr);
      lp_device != nullptr;
      lp_device = ped_device_get_next(lp_device)) {
    PedDiskType* disk_type = ped_disk_probe(lp_device);
    Device::Ptr device(new Device);
    if (disk_type == nullptr) {
      // Current device has no partition table.
      device->table = PartitionTableType::Empty;
    } else {
      const QString disk_type_name(disk_type->name);
      if (disk_type_name == kPartitionTableGPT) {
        device->table = PartitionTableType::GPT;
      } else if (disk_type_name == kPartitionTableMsDos) {
        device->table = PartitionTableType::MsDos;
      } else {
        // Ignores other type of device->
        qWarning() << "Ignores other type of device:" << lp_device->path
                   << disk_type->name;
        continue;
      }
    }

    device->path = lp_device->path;
    device->model = lp_device->model;
    device->length = lp_device->length;
    device->sector_size = lp_device->sector_size;
    device->heads = lp_device->bios_geom.heads;
    device->sectors = lp_device->bios_geom.sectors;
    device->cylinders = lp_device->bios_geom.cylinders;

    if (device->table == PartitionTableType::Empty) {
      Partition::Ptr free_partition(new Partition);
      free_partition->device_path = device->path;
      free_partition->path = "";
      free_partition->partition_number = -1;
      free_partition->start_sector = 1;
      free_partition->end_sector = device->length;
      free_partition->sector_size = device->sector_size;
      free_partition->type = PartitionType::Unallocated;
      device->partitions.append(free_partition);

    } else if (device->table == PartitionTableType::MsDos ||
        device->table == PartitionTableType::GPT) {
      PedDisk* lp_disk = nullptr;
      lp_disk = ped_disk_new(lp_device);

      if (lp_disk) {
        device->max_prims = ped_disk_get_max_primary_partition_count(lp_disk);

        // If partition table is known, scan partitions in this device->
        device->partitions = ReadPartitions(lp_disk);
        // Add additional info to partitions.
        for (Partition::Ptr partition : device->partitions) {
          partition->device_path = device->path;
          partition->sector_size = device->sector_size;
          if (!partition->path.isEmpty() &&
              partition->type != PartitionType::Unallocated) {
            // Read partition label and os.
            const QString empty_str;
            partition->label = label_items.value(partition->path, empty_str);
            for (const OsProberItem& item : os_prober_items) {
              if (item.path == partition->path) {
                partition->os = item.type;
                break;
              }
            }

            // Mark busy flag of this partition when it is mounted in system.
            for (const MountItem& mount_item : mount_items) {
              if (mount_item.path == partition->path) {
                partition->busy = true;
                break;
              }
            }
          }
        }
        ped_disk_destroy(lp_disk);

      } else {
        qCritical() << "Failed to get disk object:" << device->path;
      }
    }

    devices.append(device);
  }

  return devices;
}

}  // namespace installer
