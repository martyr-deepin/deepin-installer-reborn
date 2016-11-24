// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/models/partition_list_model.h"

#include "partman/device.h"
#include "ui/delegates/partition_delegate.h"

namespace installer {

PartitionListModel::PartitionListModel(PartitionDelegate* delegate,
                                       QObject* parent)
    : QAbstractListModel(parent),
      delegate_(delegate),
      partition_list_() {
  this->setObjectName("partition_list_model");

  connect(delegate_, &PartitionDelegate::deviceRefreshed,
          this, &PartitionListModel::onDeviceRefreshed);
}

QVariant PartitionListModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (!index.isValid()) {
    return QVariant();
  }

  const PartitionItem& item = partition_list_.at(index.row());
  if (item.recommended) {
    return tr("%1 %2 (recommended)").arg(item.path).arg(item.label);
  } else {
    return QString("%1 %2").arg(item.path).arg(item.label);
  }
}

int PartitionListModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return partition_list_.length();
}

QString PartitionListModel::getPath(const QModelIndex index) const {
  const int row = index.row();
  if (index.isValid() && row < partition_list_.length()) {
    return partition_list_.at(row).path;
  } else {
    return QString();
  }
}

QModelIndex PartitionListModel::getRecommendedIndex() const {
  for (int i = 0; i < partition_list_.length(); ++i) {
    if (partition_list_.at(i).recommended) {
      return this->index(i);
    }
  }
  return QModelIndex();
}

void PartitionListModel::onDeviceRefreshed() {
  partition_list_.clear();
  bool recommended_flag = true;
  for (const Device& device : delegate_->devices()) {
    // Use the first available device path as recommended one.
    // TODO(xushaohua): Filters removable device.
    partition_list_.append({device.path, device.model, recommended_flag});
    recommended_flag = false;

    for (const Partition& partition : device.partitions) {
      // Filters primary and logical partitions.
      if ((partition.type == PartitionType::Normal ||
          partition.type == PartitionType::Logical) &&
          !partition.path.isEmpty()) {
        // TODO(xushaohua): It is good to filter fs type.
        partition_list_.append({partition.path, "", false});
      }
    }
  }
  emit this->rowChanged();
}

}  // namespace installer