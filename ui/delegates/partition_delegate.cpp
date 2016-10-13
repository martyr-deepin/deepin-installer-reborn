// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/partition_delegate.h"

#include <QDebug>
#include <QThread>

#include "partman/partition_manager.h"
#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "service/signal_manager.h"

namespace installer {

namespace {

const char kMountPointUnused[] = "unused";

}  // namespace

PartitionDelegate::PartitionDelegate(QObject* parent)
    : QObject(parent),
      partition_manager_(new PartitionManager()),
      partition_thread_(new QThread()),
      devices_(),
      real_devices_(),
      operations_(),
      all_mount_points_(),
      unused_mount_points_(),
      fs_types_() {
  this->setObjectName(QStringLiteral("partition_delegate"));

  partition_manager_->moveToThread(partition_thread_);
  partition_thread_->start();

  this->initConnections();
}

PartitionDelegate::~PartitionDelegate() {
  delete partition_manager_;
  partition_manager_ = nullptr;

  // Quit background thread explicitly.
  partition_thread_->quit();
  partition_thread_->wait();
  delete partition_thread_;
  partition_thread_ = nullptr;
}

void PartitionDelegate::autoConf() {
  const QString script_path = GetAutoPartFile();
  emit partition_manager_->autoPart(script_path);
}

bool PartitionDelegate::canAddPrimary(const Partition& partition) const {
  const int index = DeviceIndex(devices_, partition.device_path);
  if (index == -1) {
    qCritical() << "getSupportedPartitionType() no device found at:"
                << partition.device_path;
    return false;
  }
  const Device& device = devices_.at(index);
  const PartitionList prim_partitions = GetPrimaryPartitions(device.partitions);
  const PartitionList logical_partitions = GetLogicalPartitions(
      device.partitions);

  // Check primary type
  bool primary_ok = true;
  if (prim_partitions.length() >= device.max_prims) {
    primary_ok = false;
  } else {
    // Check whether |partition| is between two logical partitions.
    bool has_logical_before = false;
    bool has_logical_after = false;
    for (const Partition& logical_partition : logical_partitions) {
      if (logical_partition.start_sector < partition.start_sector) {
        has_logical_before = true;
      }
      if (logical_partition.end_sector > partition.end_sector) {
        has_logical_after = true;
      }
    }
    if (has_logical_after && has_logical_before) {
      primary_ok = false;
    }
  }

  return primary_ok;
}

bool PartitionDelegate::canAddLogical(const Partition& partition) const {
  const int index = DeviceIndex(devices_, partition.device_path);
  if (index == -1) {
    qCritical() << "getSupportedPartitionType() no device found at:"
                << partition.device_path;
    return false;
  }
  const Device& device = devices_.at(index);

  // Ignores gpt table.
  if (device.table != PartitionTableType::MsDos) {
    return false;
  }

  bool logical_ok = true;
  const int ext_index = ExtendedPartitionIndex(device.partitions);
  if (ext_index == -1) {
    // No extended partition found, so check one more primary partition is
    // available or not.
    if (GetPrimaryPartitions(device.partitions).length() >= device.max_prims) {
      logical_ok = false;
    }
  } else {
    // Check whether there is primary partition between |partition| and
    // extended partition.
    const Partition ext_partition = device.partitions.at(ext_index);
    const PartitionList prim_partitions = GetPrimaryPartitions(
        device.partitions);
    if (partition.end_sector < ext_partition.start_sector) {
      for (const Partition& prim_partition : prim_partitions) {
        if (prim_partition.end_sector > partition.start_sector &&
            prim_partition.start_sector < ext_partition.start_sector) {
          logical_ok = false;
        }
      }
    } else if (partition.start_sector > ext_partition.end_sector) {
      for (const Partition& prim_partition : prim_partitions) {
        if (prim_partition.end_sector < partition.start_sector &&
            prim_partition.start_sector > ext_partition.end_sector) {
          logical_ok =false;
        }
      }
    }
  }
  return logical_ok;
}

void PartitionDelegate::scanDevices() const {
  // If auto-part is not set, scan devices right now.
  if (!GetSettingsBool(kPartitionDoAutoPart)) {
    emit partition_manager_->refreshDevices();
  }
}

const QStringList& PartitionDelegate::getMountPoints() {
  if (all_mount_points_.isEmpty()) {
    // Read available mount points.
    QString name = GetSettingsString(kPartitionMountPoints);
    Q_ASSERT(!name.isEmpty());
    if (name == kMountPointUnused) {
      name = "";
    }
    all_mount_points_ = name.split(';');
    unused_mount_points_ = all_mount_points_;
  }

  return unused_mount_points_;
}

void PartitionDelegate::useMountPoint(const QString& mount_point) {
  qDebug() << "useMountPoint()" << mount_point << unused_mount_points_;
//  Q_ASSERT(unused_mount_points_.contains(mount_point));
  unused_mount_points_.removeOne(mount_point);
}

const FsTypeList& PartitionDelegate::getFsTypes() {
  if (fs_types_.isEmpty()) {
    const QString name = GetSettingsString(kPartitionSupportedFs);
    Q_ASSERT(!name.isEmpty());
    const QStringList fs_names = name.split(';');
    for (const QString& fs_name : fs_names) {
      FsType type = GetFsTypeByName(fs_name);
      fs_types_.append(type);
    }
  }
  return fs_types_;
}

void PartitionDelegate::createPartition(const Partition& partition,
                                        PartitionType partition_type,
                                        bool align_start,
                                        FsType fs_type,
                                        const QString& mount_point,
                                        qint64 total_sectors) {
  Partition new_partition;
  new_partition.device_path = partition.device_path;
  new_partition.path = partition.path;
  new_partition.sector_size = partition.sector_size;
  new_partition.status = PartitionStatus::New;

  // Take into consideration if preceding space.
  const qint64 preceding_sectors = getPartitionPreceding(partition) /
                                   partition.sector_size;
  const qint64 actual_start_sector = partition.start_sector + preceding_sectors;

  if (align_start) {
    // Align from start of |partition|.
    new_partition.start_sector = actual_start_sector;
    new_partition.end_sector = total_sectors + partition.start_sector;
    new_partition.sectors_unallocated_succeeding = partition.end_sector -
                                                   new_partition.end_sector;
  } else {
    new_partition.end_sector = partition.end_sector;
    new_partition.start_sector = qMax(partition.end_sector - total_sectors,
                                      actual_start_sector);
    const qint64 start_sector_delta = new_partition.start_sector -
                                      partition.start_sector;
    if (start_sector_delta > preceding_sectors) {
      new_partition.sectors_unallocated_preceding = start_sector_delta;
    }
  }

  new_partition.type = partition_type;
  new_partition.fs = fs_type;
  new_partition.mount_point = mount_point;
  Operation operation(OperationType::Create, partition, new_partition);
  operations_.append(operation);

  // TODO(xushaohua): Resize extended partition if needed

  const int device_index = DeviceIndex(devices_, partition.device_path);
  if (device_index == -1) {
    // TODO(xushaohua): Raise error msg
    qCritical() << "createPartition() device index out of range:"
                << partition.device_path;
    return;
  }
  const Device& device = devices_.at(device_index);

  const int ext_index = ExtendedPartitionIndex(device.partitions);
  if (ext_index == -1 && partition_type == PartitionType::Logical) {
    // No extended partition found, create one if needed.
    // TODO(xushaohua): Create extended partition.
  }

//  const Partition& ext_partition = device.partitions.at(ext_index);

  // If new primary partition is contained in or intersected with
  // extended partition, shrink extended partition or delete it if no other
  // logical partitions.
  // If new logical partition is not contained in or is intersected with
  // extended partition, enlarge extended partition.

  refreshVisual();
}

void PartitionDelegate::deletePartition(const Partition& partition) {
  Partition new_partition;
  new_partition.sector_size = partition.sector_size;
  new_partition.start_sector = partition.start_sector;
  new_partition.end_sector = partition.end_sector;
  new_partition.device_path = partition.device_path;
  new_partition.type = PartitionType::Unallocated;
  new_partition.freespace = new_partition.length;
  new_partition.fs = FsType::Empty;
  if (partition.status == PartitionStatus::New) {
    // Merge operations here.
  }

  Operation operation(OperationType::Delete, partition, new_partition);
  operations_.append(operation);

  if (partition.type == PartitionType::Logical) {
    // Delete extended partition if needed.
    const int device_index = DeviceIndex(devices_, partition.device_path);
    if (device_index == -1) {
      qCritical() << "deletePartition() Failed to get device:"
                  << partition.device_path;
    } else {
      const Device& device = devices_.at(device_index);
      const PartitionList logical_partitions =
          GetLogicalPartitions(device.partitions);
      if (logical_partitions.length() == 0) {
        qDebug() << "deletePartition() delete ext partition!";
        const int ext_index = ExtendedPartitionIndex(device.partitions);
        Q_ASSERT(ext_index > -1);
        Partition ext_partition = device.partitions.at(ext_index);
        qDebug() << "ext partition:" << ext_partition.path;
        Partition empty_partition = ext_partition;
        empty_partition.type = PartitionType::Unallocated;
        Operation delete_operation(OperationType::Delete, ext_partition,
                                   empty_partition);
        operations_.append(delete_operation);
      }
    }
  }

  this->refreshVisual();
}

void PartitionDelegate::formatPartition(const Partition& partition,
                                        FsType fs_type,
                                        const QString& mount_point) {
  qDebug() << "formatPartition()" << partition.path << mount_point;
  Partition new_partition;
  new_partition.sector_size = partition.sector_size;
  new_partition.start_sector = partition.start_sector;
  new_partition.end_sector = partition.end_sector;
  new_partition.path = partition.path;
  new_partition.device_path = partition.device_path;
  new_partition.fs = fs_type;
  new_partition.mount_point = mount_point;
  new_partition.status = PartitionStatus::Formatted;
  Operation operation(OperationType::Format, partition, new_partition);
  operations_.append(operation);
}

void PartitionDelegate::updateMountPoint(const Partition& partition,
                                         const QString& mount_point) {
  qDebug() << "PartitionDelegate::updateMountPoint()" << partition.path
           << mount_point;
  Partition new_partition(partition);
  new_partition.mount_point = mount_point;
  // No need to update partition status.
  Operation operation(OperationType::MountPoint, partition, new_partition);
  operations_.append(operation);
  this->refreshVisual();
}

void PartitionDelegate::doManualPart() {
  emit partition_manager_->manualPart(operations_);
}

qint64 PartitionDelegate::getPartitionPreceding(const Partition& partition) {
  const int device_index = DeviceIndex(devices_, partition.device_path);
  Q_ASSERT(device_index > -1);
  const Device& device = devices_.at(device_index);

  // If it is the first primary partition, add 1Mib.
  const int index = PartitionIndex(device.partitions, partition);
  Q_ASSERT(index > -1);
  if (index == 0) {
    return 1 * kMebiByte;
  } else {
    return 0;
  }
  // TODO(xushaohua): Handles extended partition
}

void PartitionDelegate::initConnections() {
  SignalManager* signal_manager = SignalManager::instance();
  connect(partition_manager_, &PartitionManager::autoPartDone,
          signal_manager, &SignalManager::autoPartDone);
  connect(partition_manager_, &PartitionManager::manualPartDone,
          this, &PartitionDelegate::onManualPartDone);

  connect(partition_manager_, &PartitionManager::devicesRefreshed,
          this, &PartitionDelegate::onDevicesRefreshed);
}

void PartitionDelegate::refreshVisual() {
  this->devices_ = this->real_devices_;
  for (Device& device : this->devices_) {
    for (Operation& operation : operations_) {
      if (operation.orig_partition.device_path == device.path) {
        operation.applyToVisual(device.partitions);
      }
    }
  }
  emit this->deviceRefreshed();
}

void PartitionDelegate::onDevicesRefreshed(const DeviceList& devices) {
  this->real_devices_ = devices;
  this->devices_ = devices;
  for (Device& device : devices_) {
    PartitionList new_partitions;
    for (const Partition& partition : device.partitions) {
      // Filters freespace partition based on size.
      if (partition.type == PartitionType::Unallocated &&
          partition.getByteLength() < kMinimumPartitionSizeToDisplay) {
        continue;
      }
      new_partitions.append(partition);
    }
    device.partitions = new_partitions;
  }

  emit this->deviceRefreshed();
}

void PartitionDelegate::onManualPartDone(bool ok,
                                         const QStringList& mount_point_pair) {
  if (ok) {
    // Write mount-point pair to conf.
    QString root_path;
    QString mount_points;
    for (int i = 0; i < mount_point_pair.length(); i += 2) {
      if (mount_point_pair.at(i) == kMountPointRoot) {
        root_path = mount_point_pair.at(i);
      }
      mount_points += QString("%1=%2;").arg(mount_point_pair.at(i),
                                            mount_point_pair.at(i+1));
    }

    // TODO(xushaohua): Remove the last semicolon.
    WritePartitionInfo(root_path, mount_points);
  }

  emit SignalManager::instance()->manualPartDone(ok);
}

}  // namespace installer
