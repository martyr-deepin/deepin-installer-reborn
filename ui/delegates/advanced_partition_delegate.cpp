// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/advanced_partition_delegate.h"

#include "ui/models/partition_model.h"

namespace installer {

AdvancedPartitionDelegate::AdvancedPartitionDelegate(PartitionModel* model,
                                                     QObject* parent)
    : QObject(parent),
      model_(model),
      real_devices_(),
      virtual_devices_() {
  this->setObjectName("advanced_partition_delegate");
}

bool AdvancedPartitionDelegate::canAddPrimary(
    const Partition& partition) const {
  Q_UNUSED(partition);
  return false;
}

bool AdvancedPartitionDelegate::canAddLogical(
    const Partition& partition) const {
  Q_UNUSED(partition);
  return false;
}

FsTypeList AdvancedPartitionDelegate::getFsTypeList() const {
  return installer::FsTypeList();
}

QStringList AdvancedPartitionDelegate::getMountPoints() const {
  return QStringList();
}

Partition AdvancedPartitionDelegate::getRealPartition(
    const Partition& partition) const {
  Q_UNUSED(partition);
  return Partition();
}

bool AdvancedPartitionDelegate::isPartitionTableMatch(
    const QString& device_path) const {
  Q_UNUSED(device_path);
  return false;
}

void AdvancedPartitionDelegate::createPartition(const Partition& partition,
                                                PartitionType partition_type,
                                                bool align_start,
                                                FsType fs_type,
                                                const QString& mount_point,
                                                qint64 total_sectors) {
  Q_UNUSED(partition);
  Q_UNUSED(partition_type);
  Q_UNUSED(align_start);
  Q_UNUSED(fs_type);
  Q_UNUSED(mount_point);
  Q_UNUSED(total_sectors);
}

void AdvancedPartitionDelegate::formatPartition(const Partition& partition,
                                                FsType fs_type,
                                                const QString& mount_point) {
  Q_UNUSED(partition);
  Q_UNUSED(fs_type);
  Q_UNUSED(mount_point);
}

void AdvancedPartitionDelegate::refreshVisual() {

}

void AdvancedPartitionDelegate::setBootloaderPath(const QString& path) {
  Q_UNUSED(path);
}

void AdvancedPartitionDelegate::unFormatPartition(const Partition& partition) {
  Q_UNUSED(partition);
}

void AdvancedPartitionDelegate::updateMountPoint(const Partition& partition,
                                                 const QString& mount_point) {
  Q_UNUSED(partition);
  Q_UNUSED(mount_point);
}

}  // namespace installer