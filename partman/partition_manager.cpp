// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "partition_manager.h"

#include <parted/parted.h>
#include <string.h>
#include <QDebug>
#include <QFile>

#include "base/command.h"
#include "partman/fs.h"
#include "partman/partition.h"
#include "partman/partition_label.h"
#include "partman/partition_usage.h"
#include "service/settings_manager.h"
#include "sysinfo/dev_uuid.h"

namespace partman {

namespace {

// Flush linux kernel caches. Ensure coherency between the caches of the whole
// disk device and the partition devices.
//bool FlushDevice(PedDevice* device) {
//  bool success = false;
//  if (ped_device_open(device)) {
//    success = static_cast<bool>(ped_device_sync(device));
//    ped_device_close(device);
//  }
//  return success;
//}

// Refers: http://stackoverflow.com/questions/14127210/
// After the kernel boots, `udevd` is used to create device nodes for
// all detected devices. That is a relatively time consuming task that
// has to be completed for the boot process to continue, otherwise
// there is a risk of services failing due to missing device nodes.
// `udevadm settle` waits for udevd to process the device creation events
// for all hardware devices, thus ensuring that any device nodes
// have been created successfully before proceeding.
//void SettleDevice(int timeout) {
//  const QString cmd = QString("udevadm settle --timeout=%1").arg(timeout);
//  // TODO(xushaohua):
//  Q_UNUSED(cmd);
//}

//bool CommitDiskChanges(PedDisk* disk) {
//  bool success = static_cast<bool>(ped_disk_commit_to_dev(disk));
//  if (success) {
//    success = static_cast<bool>(ped_disk_commit_to_os(disk));
//  }
//  return success;
//}

void ReadPartitions(Device& device, PedDisk* lp_disk,
                    const sysinfo::UUIDItems& uuid_items) {
  for (PedPartition* lp_partition = ped_disk_next_partition(lp_disk, NULL);
      lp_partition != NULL;
      lp_partition = ped_disk_next_partition(lp_disk, lp_partition)) {
    qDebug() << "============================";

    // Skip useless partitions.
    if (lp_partition->type & PED_PARTITION_METADATA) {
      continue;
    }

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
      partition.type = PartitionType::LogicalUnallocated;
    } else if (lp_partition->type == PED_PARTITION_LOGICAL) {
      partition.type = PartitionType::Logical;
      qDebug() << "logical";
    } else if (lp_partition->type == PED_PARTITION_FREESPACE) {
      partition.type = PartitionType::Unallocated;
      qDebug() << "freespace";
    } else {
      qDebug() << "unknown partition type" << lp_partition->type;
    }
    partition.device_path = device.path;

    if (lp_partition->fs_type) {
      partition.fs = GetFsTypeByName(lp_partition->fs_type->name);
    } else {
      partition.fs = FsType::Unknown;
    }
    partition.sector_start = lp_partition->geom.start;
    qDebug() << "sector start:" << partition.sector_start;
    partition.sectors_total = lp_partition->geom.length;
    qDebug() << "sector total:" << partition.sectors_total;
    partition.sector_end = lp_partition->geom.end;
    qDebug() << "sector end:" << partition.sector_end;
    partition.path = ped_partition_get_path(lp_partition);
    qDebug() << "partition path:" << partition.path;
    // Avoid reading additional filesystem information if there is no path.
    if (!partition.path.isEmpty() &&
        partition.type != PartitionType::Unallocated) {
      ReadLabel(partition);
      qDebug() << "partition label:" << partition.label;

      // Read uuid from /dev/disk/by-uuid/
      partition.uuid = uuid_items.value(partition.path);
      qDebug() << "UUID:" << partition.uuid;

      // Read label based on filesystem type
      ReadUsage(partition);
      qDebug() << "length:" << partition.length
               << ",freespace:" << partition.freespace;

      // Read possible Operating System version by calling `os-prober`.
    }
    //partition.flags = {};

    // If there is and end, there is a partition.
    if (partition.sector_end > -1) {
      device.partitions.append(partition);
    }
  }
}

}  // namespace

PartitionManager::PartitionManager(QObject* parent) : QObject(parent) {
  this->setObjectName(QStringLiteral("partition_manager"));

  qRegisterMetaType<DeviceList>("DeviceList");
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
  // 2. List partitions of each device.
  // 3. Retrieve partition metadata.

  // Let libparted detect all devices and construct device list.
  ped_device_probe_all();

  DeviceList devices;
  const sysinfo::UUIDItems uuid_items = sysinfo::ParseUUIDDir();

  PedDevice* lp_device = ped_device_get_next(NULL);
  while (lp_device != NULL) {
    PedDiskType* disk_type = ped_disk_probe(lp_device);
    PedDisk* lp_disk = NULL;
    // TODO(xushaohua): Filters USB device.
    if (disk_type == NULL) {
      // Raw disk found.
      // TODO(xushaohua): Check proper partition type.
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
    ReadPartitions(device, lp_disk, uuid_items);

    devices.append(device);
    //ped_disk_print(disk);
//    ped_device_destroy(p_device);
//    ped_disk_destroy(disk);

    lp_device = ped_device_get_next(lp_device);
  }

  // TODO(xushaohua): Added this->devices.
  emit this->devicesRefreshed(devices);
}

void PartitionManager::doAutoPart() {
  const QString filepath = service::GetAutoPartFile();
  if (!QFile::exists(filepath)) {
    qCritical() << "partition script file not found!";
    emit this->autoPartDone(false);
    return;
  }
  const bool ok = base::RunScriptFile(filepath);
  emit this->autoPartDone(ok);
}

void PartitionManager::doManualPart() {
  bool ok = true;
  // TODO(xushaohua):
  emit this->manualPartDone(ok);
}

}  // namespace partman
