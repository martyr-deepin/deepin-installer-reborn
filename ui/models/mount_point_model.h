// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_MODELS_MOUNT_POINT_MODEL_H
#define INSTALLER_UI_MODELS_MOUNT_POINT_MODEL_H

#include <QAbstractListModel>

namespace installer {

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

 private:
  PartitionDelegate* delegate_ = nullptr;

 private slots:
  // Update mount point list in model.
  void onMountPointUpdated();
};

// Check whether |mount_point| is empty mount point.
bool IsEmptyMountPoint(const QString& mount_point);

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_MOUNT_POINT_MODEL_H
