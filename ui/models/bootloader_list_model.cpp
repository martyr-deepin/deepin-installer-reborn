// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/models/bootloader_list_model.h"

#include "partman/device.h"
#include "ui/delegates/partition_delegate.h"
#include "ui/delegates/partition_util.h"

namespace installer {

BootloaderListModel::BootloaderListModel(PartitionDelegate* delegate,
                                       QObject* parent)
    : QAbstractListModel(parent),
      delegate_(delegate),
      bootloader_list_() {
  this->setObjectName("bootloader_list_model");

  connect(delegate_, &PartitionDelegate::deviceRefreshed,
          this, &BootloaderListModel::onDeviceRefreshed);
}

QVariant BootloaderListModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (!index.isValid()) {
    return QVariant();
  }

  const PartitionItem& item = bootloader_list_.at(index.row());
  if (item.recommended) {
    // Add "recommended" text.
    return tr("%1 %2 (recommended)").arg(item.path).arg(item.label);
  } else {
    return QString("%1 %2").arg(item.path).arg(item.label);
  }
}

int BootloaderListModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return bootloader_list_.length();
}

QString BootloaderListModel::getPath(const QModelIndex index) const {
  const int row = index.row();
  if (index.isValid() && row < bootloader_list_.length()) {
    return bootloader_list_.at(row).path;
  } else {
    return QString();
  }
}

QModelIndex BootloaderListModel::getRecommendedIndex() const {
  for (int i = 0; i < bootloader_list_.length(); ++i) {
    if (bootloader_list_.at(i).recommended) {
      return this->index(i);
    }
  }
  return QModelIndex();
}

void BootloaderListModel::onDeviceRefreshed() {
  bootloader_list_.clear();

  const QString installer_device_path(GetInstallerDevicePath());
  const DeviceList& devices = delegate_->devices();

  QString boot_device;
  QString root_device;
  QString first_device;

  for (const Device& device : devices) {
    // Ignores installer device.
    if (installer_device_path.startsWith(device.path)) {
      qDebug() << "Ignore installer device:" << installer_device_path;
      continue;
    }

    if (first_device.isEmpty()) {
      first_device = device.path;
    }

    for (const Partition& partition : device.partitions) {
      if (partition.mount_point == kMountPointRoot) {
        root_device = partition.device_path;
      } else if (partition.mount_point == kMountPointBoot) {
        boot_device = partition.device_path;
      }
    }
  }

  // Get recommended device path.
  QString recommended_device;
  if (!boot_device.isEmpty()) {
    recommended_device = boot_device;
  } else if (!root_device.isEmpty()) {
    recommended_device = root_device;
  } else {
    recommended_device = first_device;
  }

  for (const Device& device : devices) {
    // Ignores installer device.
    if (installer_device_path.startsWith(device.path)) {
      qDebug() << "Ignore installer device:" << installer_device_path;
      continue;
    }

    // Set recommended flag to false.
    const bool is_recommended = (device.path == recommended_device);
    bootloader_list_.append({device.path,
                             GetDeviceModelAndCap(device),
                             is_recommended});
    for (const Partition& partition : device.partitions) {
      // Filters primary and logical partitions.
      if ((partition.type == PartitionType::Normal ||
           partition.type == PartitionType::Logical) &&
          !partition.path.isEmpty() && !partition.busy) {
        // TODO(xushaohua): It is better to filter fs type.
        bootloader_list_.append({partition.path, "", false});
      }
    }
  }

  emit this->rowChanged();
}

}  // namespace installer