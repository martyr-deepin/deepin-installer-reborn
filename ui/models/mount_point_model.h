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

#ifndef INSTALLER_UI_MODELS_MOUNT_POINT_MODEL_H
#define INSTALLER_UI_MODELS_MOUNT_POINT_MODEL_H

#include <QAbstractListModel>

namespace installer {

// ModelPointModel always displays all available mount-points.
// Even if it is already in use.
class MountPointModel : public QAbstractListModel {
  Q_OBJECT

 public:
  MountPointModel(const QStringList& mount_points, QObject* parent = nullptr);

  QVariant data(const QModelIndex& index, int role) const override;
  int rowCount(const QModelIndex& parent) const override;

  // Get mount_point at |index|.
  QString getMountPoint(int index) const;

  // Get index of |mount_point|, if not found returns -1.
  int index(const QString& mount_point) const;

 private:
  const QStringList mount_points_;
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_MOUNT_POINT_MODEL_H
