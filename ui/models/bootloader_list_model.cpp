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
  bool recommended_flag = true;
  for (const Device& device : delegate_->devices()) {
    // Use the first available device path as recommended one.
    // TODO(xushaohua): Filters removable device.
    bootloader_list_.append({device.path,
                            GetDeviceModelAndCap(device),
                            recommended_flag});
    recommended_flag = false;

    for (const Partition& partition : device.partitions) {
      // Filters primary and logical partitions.
      if ((partition.type == PartitionType::Normal ||
          partition.type == PartitionType::Logical) &&
          !partition.path.isEmpty() && !partition.busy) {
        // TODO(xushaohua): It is good to filter fs type.
        bootloader_list_.append({partition.path, "", false});
      }
    }
  }
  emit this->rowChanged();
}

}  // namespace installer