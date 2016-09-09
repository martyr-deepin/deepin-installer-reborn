// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef DEEPIN_INSTALLER_REBORN_UI_FRAMES_MODELS_MOUNT_POINT_MODEL_H
#define DEEPIN_INSTALLER_REBORN_UI_FRAMES_MODELS_MOUNT_POINT_MODEL_H

#include <QAbstractListModel>

namespace ui {

class PartitionDelegate;

class MountPointModel : public QAbstractListModel {
  Q_OBJECT

 public:
  MountPointModel(PartitionDelegate* delegate, QObject* parent = nullptr);

  virtual int rowCount(const QModelIndex& parent) const override;

  virtual QVariant data(const QModelIndex& index, int role) const override;

 private:
  PartitionDelegate* delegate_ = nullptr;
  QStringList mount_points_;
};

}  // namespace ui

#endif  // DEEPIN_INSTALLER_REBORN_UI_FRAMES_MODELS_MOUNT_POINT_MODEL_H
