// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "mount_point_model.h"

#include "partman/structs.h"
#include "ui/delegates/partition_delegate.h"

namespace ui {

MountPointModel::MountPointModel(PartitionDelegate* delegate, QObject* parent)
    : QAbstractListModel(parent),
      delegate_(delegate) {
  this->setObjectName(QStringLiteral("mount_point_model"));
}

void MountPointModel::useMountPoint(const QString& mount_point) {
  delegate_->useMountPoint(mount_point);
}

int MountPointModel::index(const QString& mount_point) const {
  return delegate_->getMountPoints().indexOf(mount_point);
}

int MountPointModel::indexOfEmpty() const {
  return delegate_->getMountPoints().indexOf(
      partman::kPartitionMountPointUnused);
}

int MountPointModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return delegate_->getMountPoints().length();
}

QVariant MountPointModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (!index.isValid()) {
    return QVariant();
  }

  const QString name =  delegate_->getMountPoints().at(index.row());
  if (name == partman::kPartitionMountPointUnused) {
    // TODO(xushaohua): Check filesystem type of partition.
    return tr("do not use");
  } else {
    return name;
  }
}

bool IsEmptyMountPoint(const QString& mount_point) {
  return mount_point == partman::kPartitionMountPointUnused;
}

}  // namespace ui