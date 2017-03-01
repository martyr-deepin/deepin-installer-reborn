// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/simple_partition_delegate.h"

#include "ui/models/partition_model.h"

namespace installer {

SimplePartitionDelegate::SimplePartitionDelegate(PartitionModel* model,
                                                 QObject* parent)
    : QObject(parent),
      model_(model),
      real_devices_(),
      virtual_devices_() {
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
  Q_UNUSED(device_path);
  return false;
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

void SimplePartitionDelegate::setBootloaderPath(const QString& path) {
  Q_UNUSED(path);
}

}  // namespace installer