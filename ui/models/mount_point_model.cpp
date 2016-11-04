// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "mount_point_model.h"

#include "ui/delegates/partition_delegate.h"

namespace installer {

MountPointModel::MountPointModel(PartitionDelegate* delegate, QObject* parent)
    : QAbstractListModel(parent),
      delegate_(delegate) {
  this->setObjectName("mount_point_model");
}

QVariant MountPointModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (!index.isValid()) {
    return QVariant();
  }

  const QStringList& mount_points(delegate_->getMountPoints());

  if (index.row() < mount_points.length()) {
    const QString name = mount_points.at(index.row());
    // TODO(xushaohua): Check filesystem type of partition.
    if (name.isEmpty()) {
      return tr("do not use");
    }
    return name;
  } else {
    return QVariant();
  }
}

int MountPointModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return delegate_->getMountPoints().length();
}

int MountPointModel::index(const QString& mount_point) const {
  return delegate_->getMountPoints().indexOf(mount_point);
}

QString MountPointModel::getMountPoint(int index) const {
  const QStringList& mount_points(delegate_->getMountPoints());
  Q_ASSERT(index < mount_points.length());
  if (index < mount_points.length()) {
    return mount_points.at(index);
  } else {
    return QString();
  }
}

void MountPointModel::useMountPoint(const QString& mount_point) {
  // Ignores empty mount point.
  if (!mount_point.isEmpty()) {
    delegate_->useMountPoint(mount_point);
  }
}

}  // namespace installer