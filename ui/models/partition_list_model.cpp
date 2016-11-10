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

  connect(delegate_, &PartitionDelegate::mountPointUpdated,
          this, &PartitionListModel::onMountPointUpdated);
}

QVariant PartitionListModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (!index.isValid()) {
    return QVariant();
  }

  return partition_list_.at(index.row());
}

int PartitionListModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return partition_list_.length();
}

void PartitionListModel::onMountPointUpdated() {
  // TODO(xushaohua): Filters only device path, /boot path and /root path.
  partition_list_.clear();
  for (const Device& device : delegate_->devices()) {
    partition_list_.append(device.path);
    for (const Partition& partition : device.partitions) {
      // Filters primary and logical partitions.
      if (partition.type == PartitionType::Normal ||
          partition.type == PartitionType::Logical) {
        partition_list_.append(partition.path);
      }
    }
  }
}

}  // namespace installer