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

namespace ui {

PartitionDelegate::PartitionDelegate(QObject* parent)
    : QObject(parent),
      partition_manager_(new partman::PartitionManager()),
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

  // If auto-part is not set, scan devices right now.
  if (!service::GetSettingsBool(service::kPartitionDoAutoPart)) {
    emit partition_manager_->refreshDevices();
  }
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
  emit partition_manager_->autoPart();
}

PartitionType PartitionDelegate::getPartitionType(
    const partman::Partition& partition) const {
  // TODO(xushaohua): Check partition layout.
  Q_UNUSED(partition);
  return PartitionType::PrimaryOnly;
}

const QStringList& PartitionDelegate::getMountPoints() {
  if (all_mount_points_.isEmpty()) {
    // Read available mount points.
    const QString name =
        service::GetSettingsString(service::kPartitionMountPoints);
    Q_ASSERT(!name.isEmpty());
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

const partman::FsTypeList& PartitionDelegate::getFsTypes() {
  if (fs_types_.isEmpty()) {
    const QString name =
        service::GetSettingsString(service::kPartitionSupportedFs);
    Q_ASSERT(!name.isEmpty());
    const QStringList fs_names = name.split(';');
    for (const QString& fs_name : fs_names) {
      partman::FsType type = partman::GetFsTypeByName(fs_name);
      fs_types_.append(type);
    }
  }
  return fs_types_;
}

void PartitionDelegate::createPartition(const partman::Partition& partition,
                                        partman::FsType fs_type,
                                        const QString& mount_point,
                                        qint64 partition_size,
                                        bool align_start) {
  partman::Partition new_partition(partition);
  new_partition.fs = fs_type;
  new_partition.mount_point = mount_point;
  new_partition.freespace = partition.length;
  // TODO(xushaohua): Calculate new partition sector size
  Q_UNUSED(partition_size);
  Q_UNUSED(align_start);
  partman::Operation operation(partman::OperationType::Create,
                               partition,
                               new_partition);
  operations_.append(operation);
  refreshVisual();
}

void PartitionDelegate::deletePartition(const partman::Partition& partition) {
  partman::Partition new_partition(partition);
  // TODO(xushaohua): check logical partition.
  new_partition.type = partman::PartitionType::Unallocated;
  new_partition.freespace = new_partition.length;
  new_partition.fs = partman::FsType::Empty;
  partman::Operation operation(partman::OperationType::Delete,
                               partition,
                               new_partition);
  operations_.append(operation);
  this->refreshVisual();
}

void PartitionDelegate::formatPartition(const partman::Partition& partition,
                                        partman::FsType fs_type,
                                        const QString& mount_point) {
  qDebug() << "formatPartition()" << partition.path << mount_point;
  partman::Partition new_partition(partition);
  new_partition.fs = fs_type;
  new_partition.mount_point = mount_point;
  new_partition.status = partman::PartitionStatus::Formatted;
  partman::Operation operation(partman::OperationType::Format,
                               partition,
                               new_partition);
  operations_.append(operation);
}

void PartitionDelegate::updateMountPoint(const partman::Partition& partition,
                                         const QString& mount_point) {
  qDebug() << "PartitionDelegate::updateMountPoint()" << partition.path
           << mount_point;
  partman::Partition partition_new(partition);
  partition_new.mount_point = mount_point;
  // No need to update partition status.
  partman::Operation operation(partman::OperationType::MountPoint,
                               partition,
                               partition_new);
  operations_.append(operation);
  this->refreshVisual();
}

void PartitionDelegate::doManualPart() {
  emit partition_manager_->manualPart(operations_);
}

void PartitionDelegate::initConnections() {
  service::SignalManager* signal_manager = service::SignalManager::instance();
  connect(partition_manager_, &partman::PartitionManager::autoPartDone,
          signal_manager, &service::SignalManager::autoPartDone);
  connect(partition_manager_, &partman::PartitionManager::manualPartDone,
          signal_manager, &service::SignalManager::manualPartDone);

  connect(partition_manager_, &partman::PartitionManager::devicesRefreshed,
          this, &PartitionDelegate::onDevicesRefreshed);
}

void PartitionDelegate::refreshVisual() {
  this->devices_ = this->real_devices_;
  for (partman::Device& device : this->devices_) {
    for (partman::Operation& operation : operations_) {
      if (operation.partition_orig.device_path == device.path) {
        operation.applyToVisual(device.partitions);
      }
    }
  }
  emit this->deviceRefreshed();
}

void PartitionDelegate::onDevicesRefreshed(const partman::DeviceList& devices) {
  this->real_devices_ = devices;
  this->devices_ = devices;
  for (partman::Device& device : devices_) {
    partman::PartitionList new_partitions;
    for (const partman::Partition& partition : device.partitions) {
      // Filters freespace partition based on size.
      if (partition.type == partman::PartitionType::Unallocated &&
          partition.getByteLength() < kMinimumPartitionSizeToDisplay) {
        continue;
      }
      new_partitions.append(partition);
    }
    device.partitions = new_partitions;
  }

  emit this->deviceRefreshed();
}

}  // namespace ui
