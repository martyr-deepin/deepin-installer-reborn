/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ui/models/bootloader_list_model.h"

#include "ui/delegates/partition_util.h"

namespace installer {

BootloaderListModel::BootloaderListModel(QObject* parent)
    : QAbstractListModel(parent),
      bootloader_list_() {
  this->setObjectName("bootloader_list_model");
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

void BootloaderListModel::onDeviceRefreshed(const DeviceList& devices) {
  this->beginResetModel();

  // Clear old bootloader list.
  bootloader_list_.clear();

  QString boot_device;
  QString root_device;
  QString first_device;

  for (const Device::Ptr device : devices) {
    if (first_device.isEmpty()) {
      first_device = device->path;
    }

    for (const Partition::Ptr partition : device->partitions) {
      if (partition->mount_point == kMountPointRoot) {
        root_device = partition->device_path;
      } else if (partition->mount_point == kMountPointBoot) {
        boot_device = partition->device_path;
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

  for (const Device::Ptr device : devices) {
    // Set recommended flag to false.
    const bool is_recommended = (device->path == recommended_device);
    bootloader_list_.append({device->path,
                             GetDeviceModelAndCap(device),
                             is_recommended});
    for (const Partition::Ptr partition : device->partitions) {
      // Filters primary and logical partitions.
      if ((partition->type == PartitionType::Normal ||
           partition->type == PartitionType::Logical) &&
          !partition->path.isEmpty() && !partition->busy) {
        // TODO(xushaohua): It is better to filter fs type.
        bootloader_list_.append({partition->path, "", false});
      }
    }
  }

  this->endResetModel();

  emit this->rowChanged();
}

}  // namespace installer
