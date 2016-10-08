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

void PartitionDelegate::scanDevices() const {
  // If auto-part is not set, scan devices right now.
  if (!GetSettingsBool(kPartitionDoAutoPart)) {
    emit partition_manager_->refreshDevices();
  }
}

SupportedPartitionType PartitionDelegate::getPartitionType(
    const Partition& partition) const {
  const int index = DeviceIndex(devices_, partition.device_path);
  Q_ASSERT(index > -1);
  const Device& device = devices_.at(index);
  if (device.table == PartitionTableType::MsDos) {
    // TODO(xushaohua): Check max_prims and logical partition
    return SupportedPartitionType::PrimaryOnly;
  } else if (device.table == PartitionTableType::GPT) {
    // GPT only support primary partitions.
    if (device.partitions.length() >= device.max_prims) {
      return SupportedPartitionType::NoMorePartitionError;
    } else {
      return SupportedPartitionType::PrimaryOnly;
    }
  } else {
    qCritical() << "getPartitionType() unknown partition table at:"
                << partition.device_path;
    return SupportedPartitionType::InvalidPartitionTable;
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
  const qint64 actual_start_sector = partition.sector_start + preceding_sectors;

  if (align_start) {
    // Align from start of |partition|.
    new_partition.sector_start = actual_start_sector;
    new_partition.sector_end = total_sectors + partition.sector_start;
    new_partition.sectors_unallocated_succeeding = partition.sector_end -
                                                   new_partition.sector_end;
  } else {
    new_partition.sector_end = partition.sector_end;
    new_partition.sector_start = qMax(partition.sector_end - total_sectors,
                                      actual_start_sector);
    const qint64 start_sector_delta = new_partition.sector_start -
                                      partition.sector_start;
    if (start_sector_delta > preceding_sectors) {
      new_partition.sectors_unallocated_preceding = start_sector_delta;
    }
  }

  new_partition.type = partition_type;
  new_partition.fs = fs_type;
  new_partition.mount_point = mount_point;
  Operation operation(OperationType::Create, partition, new_partition);
  operations_.append(operation);
  refreshVisual();
}

void PartitionDelegate::deletePartition(const Partition& partition) {
  Partition new_partition(partition);
  new_partition.type = PartitionType::Unallocated;
  new_partition.freespace = new_partition.length;
  new_partition.fs = FsType::Empty;
  if (partition.status == PartitionStatus::New) {
    // Merge operations here.
  }

  Operation operation(OperationType::Delete, partition,
                               new_partition);
  operations_.append(operation);
  this->refreshVisual();
}

void PartitionDelegate::formatPartition(const Partition& partition,
                                        FsType fs_type,
                                        const QString& mount_point) {
  qDebug() << "formatPartition()" << partition.path << mount_point;
  Partition new_partition(partition);
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

void PartitionDelegate::onManualPartDone(
    bool ok, const QList<QPair<QString, QString>>& mount_point_pair) {
  if (ok) {
    // Write mount-point pair to conf.
    QString root_path;
    QString mount_points;
    for (const QPair<QString, QString>& item : mount_point_pair) {
      if (item.second == kMountPointRoot) {
        root_path = item.first;
      }
      mount_points += QString("%1=%2;").arg(item.first, item.second);
    }
    // TODO(xushaohua): Remove the last semicolon.
    WritePartitionInfo(root_path, mount_points);
  }

  emit SignalManager::instance()->manualPartDone(ok);
}

}  // namespace installer
