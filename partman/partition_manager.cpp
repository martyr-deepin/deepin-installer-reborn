// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partition_manager.h"

#include <parted/parted.h>
#include <QDebug>
#include <QFile>

#include "base/command.h"
#include "partman/libparted_util.h"
#include "partman/os_prober.h"
#include "partman/partition_usage.h"
#include "partman/utils.h"
#include "sysinfo/dev_disk.h"

namespace installer {

namespace {

Partition GetBootPartition(const OperationList& operations) {
  for (const Operation& operation : operations) {
    // Or check mount_point == kMountPointEFI.
    if (operation.new_partition.fs == FsType::EFI) {
      return operation.new_partition;
    }
  }

  for (const Operation& operation : operations) {
    if (operation.new_partition.mount_point == kMountPointBoot) {
      return operation.new_partition;
    }
  }

  for (const Operation& operation : operations) {
    if (operation.new_partition.mount_point == kMountPointRoot) {
      return operation.new_partition;
    }
  }
  return Partition();
}

PartitionList ReadPartitions(PedDisk* lp_disk) {
  PartitionList partitions;
  for (PedPartition* lp_partition = ped_disk_next_partition(lp_disk, NULL);
      lp_partition != NULL;
      lp_partition = ped_disk_next_partition(lp_disk, lp_partition)) {
    qDebug() << "============================";

    Partition partition;
    if (lp_partition->type == PED_PARTITION_NORMAL) {
      partition.type = PartitionType::Primary;
      qDebug() << "normal";
    } else if (lp_partition->type == PED_PARTITION_EXTENDED) {
      partition.type = PartitionType::Extended;
      qDebug() << "extended";
    } else if (lp_partition->type ==
        (PED_PARTITION_FREESPACE | PED_PARTITION_LOGICAL)) {
      qDebug() << "logical freespace";
      partition.type = PartitionType::Unallocated;
    } else if (lp_partition->type == PED_PARTITION_LOGICAL) {
      partition.type = PartitionType::Logical;
      qDebug() << "logical";
    } else if (lp_partition->type == PED_PARTITION_FREESPACE) {
      partition.type = PartitionType::Unallocated;
      qDebug() << "freespace";
    } else {
      qDebug() << "unknown partition type" << lp_partition->type;
      continue;
    }

    if (lp_partition->fs_type) {
      partition.fs = GetFsTypeByName(lp_partition->fs_type->name);
      // TODO(xushaohua): Check EFI flag
      // TODO(xushaohua): Read flags
    } else {
      partition.fs = FsType::Unknown;
    }
    partition.sector_start = lp_partition->geom.start;
    qDebug() << "sector start:" << partition.sector_start;
    partition.sector_end = lp_partition->geom.end;
    qDebug() << "sector end:" << lp_partition->geom.end;

    // Result of ped_partition_get_path() need to be freed by hand.
    char* lp_path = ped_partition_get_path(lp_partition);
    if (lp_path != NULL) {
      partition.path = lp_path;
      qDebug() << "partition path:" << partition.path;
      free(lp_path);
    }

    // Avoid reading additional filesystem information if there is no path.
    if (!partition.path.isEmpty() &&
        partition.type != PartitionType::Unallocated) {

      // Read label based on filesystem type
      ReadUsage(partition.path, partition.fs, partition.freespace,
                partition.length);
      // If LinuxSwap partition is not mount, it is totally free.
      if (partition.fs == FsType::LinuxSwap && partition.length <= 0) {
        partition.length = partition.getByteLength();
        partition.freespace = partition.length;
      }
      qDebug() << "length:" << partition.length
               << ",freespace:" << partition.freespace;
    }
    partitions.append(partition);
  }
  return partitions;
}

}  // namespace

PartitionManager::PartitionManager(QObject* parent) : QObject(parent) {
  this->setObjectName(QStringLiteral("partition_manager"));

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

void PartitionManager::doRefreshDevices() {
  const DeviceList devices = ScanDevices();
  emit this->devicesRefreshed(devices);
}

void PartitionManager::doAutoPart(const QString& script_path) {
  if (!QFile::exists(script_path)) {
    qCritical() << "partition script file not found!" << script_path;
    emit this->autoPartDone(false);
    return;
  }
  const bool ok = RunScriptFile(script_path);
  emit this->autoPartDone(ok);
}

void PartitionManager::doManualPart(const OperationList& operations) {
  bool ok = true;
  for (int i = 0; ok && i < operations.length(); ++i) {
    ok = operations.at(i).applyToDisk();
  }

  if (ok) {
    // Set boot flag on boot partition.
    const Partition boot_partition = GetBootPartition(operations);
    if (boot_partition.path.isEmpty()) {
      ok = SetBootFlag(boot_partition, true);
    } else {
      qWarning() << "Failed to find boot partition!";
      ok = false;
    }
  }

  QList<QPair<QString, QString>> mount_point_pair;
  if (ok) {
    for (const Operation& operation : operations) {
      const Partition& new_partition = operation.new_partition;
      if (!new_partition.mount_point.isEmpty()) {
        mount_point_pair.append({new_partition.path,
                                 new_partition.mount_point});
      }
    }
  }

  emit this->manualPartDone(ok, mount_point_pair);
}

DeviceList ScanDevices() {
  // 1. List Devices
  // 1.1. Retrieve metadata of each device.
  // 2. List partitions of each device.
  // 3. Retrieve partition metadata.

  // Let libparted detect all devices and construct device list.
  ped_device_probe_all();

  DeviceList devices;
  const LabelItems label_items = ParseLabelDir();
  const PartLabelItems part_label_items = ParsePartLabelDir();
  const UUIDItems uuid_items = ParseUUIDDir();
  const OsTypeItems os_type_items = GetOsTypeItems();

  PedDevice* lp_device = ped_device_get_next(NULL);
  while (lp_device != NULL) {
    PedDiskType* disk_type = ped_disk_probe(lp_device);
    PedDisk* lp_disk = NULL;
    // TODO(xushaohua): Filters USB device.
    if (disk_type == NULL) {
      // Raw disk found.
      if (IsEfiEnabled()) {
        disk_type = ped_disk_type_get(kPartitionTableGPT);
      } else {
        disk_type = ped_disk_type_get(kPartitionTableMsDos);
      }
      if (disk_type) {
        // Create a new device table but not commit changes to device.
        lp_disk = ped_disk_new_fresh(lp_device, disk_type);
      } else {
        qCritical() << "ScanDevices() disk_type is NULL";
      }
    } else if (QString(kPartitionTableGPT) == disk_type->name ||
               QString(kPartitionTableMsDos) == disk_type->name) {
      lp_disk = ped_disk_new(lp_device);
    } else {
      // Ignores other type of device.
      qWarning() << "Ignores other type of device:" << disk_type->name;
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
        const QString empty_str;
        partition.label = label_items.value(partition.path, empty_str);
        partition.part_label = part_label_items.value(partition.path,
                                                      empty_str);
        partition.uuid = uuid_items.value(partition.path, empty_str);
        partition.os = os_type_items.value(partition.path, OsType::Empty);
      }
    }

    devices.append(device);
    ped_disk_destroy(lp_disk);

    lp_device = ped_device_get_next(lp_device);
  }

  return devices;
}

}  // namespace installer
