// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/simple_partition_delegate.h"

#include "ui/delegates/partition_util.h"

namespace installer {

SimplePartitionDelegate::SimplePartitionDelegate(QObject* parent)
    : QObject(parent),
      real_devices_(),
      virtual_devices_(),
      bootloader_path_() {
  this->setObjectName("simple_partition_delegate");
}

bool SimplePartitionDelegate::canAddLogical(const Partition& partition) const {
  Q_UNUSED(partition);
  return false;
}

bool SimplePartitionDelegate::canAddPrimary(const Partition& partition) const {
  Q_UNUSED(partition);
  return false;
}

bool SimplePartitionDelegate::isPartitionTableMatch(
    const QString& device_path) const {
  return IsPartitionTableMatch(real_devices_, device_path);
}

void SimplePartitionDelegate::resetOperations() {

}

void SimplePartitionDelegate::createPartition(const Partition& partition,
                                              PartitionType part_type,
                                              bool align_start,
                                              FsType fs_type,
                                              const QString& mount_point,
                                              qint64 total_sectors) {
  Q_UNUSED(partition);
  Q_UNUSED(part_type);
  Q_UNUSED(align_start);
  Q_UNUSED(fs_type);
  Q_UNUSED(mount_point);
  Q_UNUSED(total_sectors);
}

Partition SimplePartitionDelegate::deletePartition(const Partition& partition) {
  Q_UNUSED(partition);
  return Partition();
}

void SimplePartitionDelegate::formatPartition(const Partition& partition,
                                              FsType fs_type,
                                              const QString& mount_point) {
  Q_UNUSED(partition);
  Q_UNUSED(fs_type);
  Q_UNUSED(mount_point);
}

void SimplePartitionDelegate::onDeviceRefreshed(const DeviceList& devices) {
  real_devices_ = devices;
  // TODO(xushaohua): Update virtual device list.
}

void SimplePartitionDelegate::setBootloaderPath(const QString& path) {
  bootloader_path_ = path;
}

}  // namespace installer