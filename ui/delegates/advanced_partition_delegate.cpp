// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/delegates/advanced_partition_delegate.h"

#include "service/settings_manager.h"
#include "service/settings_name.h"
#include "ui/delegates/partition_util.h"

namespace installer {

namespace {

// "unused" mount point.
const char kMountPointUnused[] = "unused";

}  // namespace

AdvancedPartitionDelegate::AdvancedPartitionDelegate(QObject* parent)
    : QObject(parent),
      real_devices_(),
      virtual_devices_(),
      bootloader_path_(),
      operations_() {
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
  FsTypeList fs_types;
  if (fs_types.isEmpty()) {
    const QString name = GetSettingsString(kPartitionSupportedFs);
    Q_ASSERT(!name.isEmpty());
    const QStringList fs_names = name.split(';');
    for (const QString& fs_name : fs_names) {
      FsType type = GetFsTypeByName(fs_name);
      fs_types.append(type);
    }
  }
  return fs_types;
}

QStringList AdvancedPartitionDelegate::getMountPoints() const {
  QStringList mount_points;
  if (mount_points.isEmpty()) {
    // Read available mount points.
    mount_points = GetSettingsStringList(kPartitionMountPoints);

    // Replace "unused" mount point with ""
    for (QString& mount_point : mount_points) {
      if (mount_point == kMountPointUnused) {
        mount_point = "";
      }
    }
  }
  return mount_points;
}

Partition AdvancedPartitionDelegate::getRealPartition(
    const Partition& virtual_partition) const {
  const int index = DeviceIndex(real_devices_, virtual_partition.device_path);
  if (index == -1) {
    qWarning() << "failed to find device:" << virtual_partition.device_path;
    return Partition();
  }

  for (const Partition& partition : real_devices_.at(index).partitions) {
    if (partition == virtual_partition) {
      return partition;
    }
  }

  qWarning() << "Failed to find partition at:" << virtual_partition;
  return Partition();
}

bool AdvancedPartitionDelegate::isPartitionTableMatch(
    const QString& device_path) const {
  return IsPartitionTableMatch(real_devices_, device_path);
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
  bootloader_path_ = path;
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