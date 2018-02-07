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

QString MountPointModel::getMountPoint(int index) const {
  Q_ASSERT(index < mount_points_.length());
  if (index >= 0 && index < mount_points_.length()) {
    return mount_points_.at(index);
  } else {
    return QString();
  }
}

int MountPointModel::index(const QString& mount_point) const {
  return mount_points_.indexOf(mount_point);
}

}  // namespace installer