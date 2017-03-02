// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "mount_point_model.h"

namespace installer {

MountPointModel::MountPointModel(const QStringList& mount_points,
                                 QObject* parent)
    : QAbstractListModel(parent),
      mount_points_(mount_points) {
  this->setObjectName("mount_point_model");
}

QVariant MountPointModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (index.isValid()) {
    const QString name = mount_points_.at(index.row());
    if (name.isEmpty()) {
      return tr("Do not use");
    } else {
      return name;
    }
  }

  return QVariant();
}

int MountPointModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return mount_points_.length();
}

int MountPointModel::index(const QString& mount_point) const {
  return mount_points_.indexOf(mount_point);
}

QString MountPointModel::getMountPoint(int index) const {
  Q_ASSERT(index < mount_points_.length());
  if (index >= 0 && index < mount_points_.length()) {
    return mount_points_.at(index);
  } else {
    return QString();
  }
}

}  // namespace installer