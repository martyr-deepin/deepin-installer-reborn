// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

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

  // Get index of |mount_point|, if not found returns -1.
  int index(const QString& mount_point) const;

  // Get mount_point at |index|.
  QString getMountPoint(int index) const;

 private:
  const QStringList mount_points_;
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_MOUNT_POINT_MODEL_H
