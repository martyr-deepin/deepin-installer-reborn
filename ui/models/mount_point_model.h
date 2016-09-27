// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_MODELS_MOUNT_POINT_MODEL_H
#define DEEPIN_INSTALLER_REBORN_UI_MODELS_MOUNT_POINT_MODEL_H

#include <QAbstractListModel>

namespace ui {

class PartitionDelegate;

class MountPointModel : public QAbstractListModel {
  Q_OBJECT

 public:
  MountPointModel(PartitionDelegate* delegate, QObject* parent = nullptr);

  QVariant data(const QModelIndex& index, int role) const override;
  int rowCount(const QModelIndex& parent) const override;

  // Get index of |mount_point|, if not found returns -1.
  int index(const QString& mount_point) const;
  // Get mount_point at |index|.
  QString getMountPoint(int index) const;

  // Take |mount_point| out of unused-mount-point list.
  void useMountPoint(const QString& mount_point);

 private:
  PartitionDelegate* delegate_ = nullptr;
};

// Check whether |mount_point| is empty mount point.
bool IsEmptyMountPoint(const QString& mount_point);

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_MODELS_MOUNT_POINT_MODEL_H
