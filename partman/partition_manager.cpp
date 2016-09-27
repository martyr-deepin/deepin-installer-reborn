// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partition_manager.h"

#include <parted/parted.h>
#include <string.h>
#include <QDebug>
#include <QFile>

#include "base/command.h"
#include "partman/libparted_util.h"
#include "partman/partition_usage.h"
#include "sysinfo/dev_disk.h"

namespace partman {

namespace {

Partition GetBootPartition(const OperationList& operations) {
  for (const Operation& operation : operations) {
    // Or check mount_point == kMountPointEFI.
    if (operation.partition_new.fs == FsType::EFI) {
      return operation.partition_new;
    }
  }

  for (const Operation& operation : operations) {
    if (operation.partition_new.mount_point == kMountPointBoot) {
      return operation.partition_new;
    }
  }

  for (const Operation& operation : operations) {
    if (operation.partition_new.mount_point == kMountPointRoot) {
      return operation.partition_new;
    }
  }
  return Partition();
}

void ReadPartitions(Device& device, PedDisk* lp_disk,
                    const sysinfo::LabelItems& label_items,
                    const sysinfo::PartLabelItems& part_label_items,
                    const sysinfo::UUIDItems& uuid_items) {
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
    partition.device_path = device.path;

    if (lp_partition->fs_type) {
      partition.fs = GetFsTypeByName(lp_partition->fs_type->name);
      // TODO(xushaohua): Check EFI flag
    } else {
      partition.fs = FsType::Unknown;
    }
    partition.sector_start = lp_partition->geom.start;
    qDebug() << "sector start:" << partition.sector_start;
    partition.sector_end = lp_partition->geom.end;
    qDebug() << "sector end:" << lp_partition->geom.end;
    partition.sector_size = device.sector_size;
    partition.path = ped_partition_get_path(lp_partition);
    qDebug() << "partition path:" << partition.path;
    // Avoid reading additional filesystem information if there is no path.
    if (!partition.path.isEmpty() &&
        partition.type != PartitionType::Unallocated) {

      const QString empty_str;
      partition.label = label_items.value(partition.path, empty_str);
      partition.part_label = part_label_items.value(partition.path, empty_str);
      partition.uuid = uuid_items.value(partition.path, empty_str);
      qDebug() << "partition label:" << partition.label;
      qDebug() << "partition part-label:" << partition.part_label;
      qDebug() << "UUID:" << partition.uuid;

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

      // Read possible Operating System version by calling `os-prober`.
    }
    //partition.flags = {};

    device.partitions.append(partition);
  }
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
  // 1. List Devices
  // 1.1. Retrieve metadata of each device.
  // 2. List partitions of each device.
  // 3. Retrieve partition metadata.

  // Let libparted detect all devices and construct device list.
  ped_device_probe_all();

  DeviceList devices;
  const sysinfo::LabelItems label_items = sysinfo::ParseLabelDir();
  const sysinfo::PartLabelItems part_label_items = sysinfo::ParsePartLabelDir();
  const sysinfo::UUIDItems uuid_items = sysinfo::ParseUUIDDir();

  PedDevice* lp_device = ped_device_get_next(NULL);
  while (lp_device != NULL) {
    PedDiskType* disk_type = ped_disk_probe(lp_device);
    PedDisk* lp_disk = NULL;
    // TODO(xushaohua): Filters USB device.
    if (disk_type == NULL) {
      // Raw disk found.
      // TODO(xushaohua): Check proper partition type.
      // TODO(xushaohua): Move to libparted_util
      lp_disk = ped_disk_new_fresh(lp_device,
                                   ped_disk_type_get(kPartitionTableGPT));
      disk_type = ped_disk_probe(lp_device);
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

    device.partitions.clear();
    ReadPartitions(device, lp_disk, label_items, part_label_items, uuid_items);

    devices.append(device);
    ped_disk_destroy(lp_disk);

    lp_device = ped_device_get_next(lp_device);
  }

  emit this->devicesRefreshed(devices);
}

void PartitionManager::doAutoPart(const QString& script_path) {
  if (!QFile::exists(script_path)) {
    qCritical() << "partition script file not found!" << script_path;
    emit this->autoPartDone(false);
    return;
  }
  const bool ok = base::RunScriptFile(script_path);
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
      const Partition& partition_new = operation.partition_new;
      if (!partition_new.mount_point.isEmpty()) {
        mount_point_pair.append({partition_new.path,
                                 partition_new.mount_point});
      }
    }
  }

  emit this->manualPartDone(ok, mount_point_pair);
}

}  // namespace partman
