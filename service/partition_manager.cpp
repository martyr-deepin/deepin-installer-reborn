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

}  // namespace

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
      qDebug() << "Ignores other type of device:" << device_type->name;
      continue;
    }
    qDebug() << "path:" << p_device->path;
    qDebug() << "model:" << p_device->model;
    qDebug() << "read only:" << p_device->read_only;
    qDebug() << "sector size:" << p_device->sector_size;
    qDebug() << "partition type:" << device_type->name;
    qDebug() << "sectors:" << p_device->bios_geom.sectors;
    qDebug() << "length:" << p_device->length;

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
      partition.fs = p_partition->fs_type->name;
      //partition.freespace;
      //partition.length = ;
      partition.first_sector = p_partition->geom.start;
      partition.total_sectors = p_partition->geom.length;
      partition.last_sector = p_partition->geom.end;
      partition.path = ped_partition_get_path(p_partition);
      // Avoid reading additional filesystem information if there is no path.
      if (!partition.path.isEmpty()) {
        // Read uuid from /dev/disk/by-uuid/
        //partition.uuid;

        // Read label based on filesystem type
        //partition.label;
 
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
