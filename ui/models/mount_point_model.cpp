// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "mount_point_model.h"

#include "partman/structs.h"
#include "ui/delegates/partition_delegate.h"

namespace ui {

MountPointModel::MountPointModel(PartitionDelegate* delegate, QObject* parent)
    : QAbstractListModel(parent),
      delegate_(delegate),
      mount_points_(delegate->getMountPoints()) {
  this->setObjectName(QStringLiteral("mount_point_model"));
}

int MountPointModel::index(const QString& mount_point) const {
  return mount_points_.indexOf(mount_point);
}

int MountPointModel::indexOfEmpty() const {
  return mount_points_.indexOf(partman::kPartitionMountPointUnused);
}

int MountPointModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return mount_points_.length();
}

QVariant MountPointModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (!index.isValid()) {
    return QVariant();
  }

  const QString name =  mount_points_.at(index.row());
  if (name == partman::kPartitionMountPointUnused) {
    return tr("do not use");
  } else {
    return name;
  }
}

bool IsEmptyMountPoint(const QString& mount_point) {
  return mount_point == partman::kPartitionMountPointUnused;
}

}  // namespace ui