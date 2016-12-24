// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#ifndef INSTALLER_UI_MODELS_FS_MODEL_H
#define INSTALLER_UI_MODELS_FS_MODEL_H

#include <QAbstractListModel>

#include "partman/fs.h"

namespace installer {

class PartitionDelegate;

class FsModel : public QAbstractListModel {
  Q_OBJECT

 public:
  FsModel(PartitionDelegate* delegate, QObject* parent = nullptr);

  QVariant data(const QModelIndex& index, int role) const override;
  int rowCount(const QModelIndex& parent) const override;

  // Index fs type. Returns -1 if not found.
  int index(FsType fs_type) const;

  // Get fs type at |index|.
  FsType getFs(int index) const;

 private:
  PartitionDelegate* delegate_ = nullptr;
  FsTypeList fs_list_;
};

}  // namespace installer

#endif  // INSTALLER_UI_MODELS_FS_MODEL_H
