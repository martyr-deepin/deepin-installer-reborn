// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/frames/models/fs_model.h"

#include "ui/frames/delegates/partition_delegate.h"
#include "ui/frames/delegates/partition_util.h"

namespace ui {

FsModel::FsModel(PartitionDelegate* delegate, QObject* parent)
    : QAbstractListModel(parent),
      delegate_(delegate),
      fs_list_() {
  this->setObjectName(QStringLiteral("fs_model"));

  this->updateList();
}

int FsModel::index(partman::FsType fs_type) const {
  return fs_list_.indexOf(fs_type);
}

int FsModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return fs_list_.length();
}

QVariant FsModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (!index.isValid()) {
    return QVariant();
  }

  if (index.row() >= fs_list_.length()) {
    return QVariant();
  }

  const partman::FsType fs = fs_list_.at(index.row());
  return GetLocalFsTypeName(fs);
}

void FsModel::updateList() {
  fs_list_.clear();
  fs_list_.append(delegate_->getFsTypes());
  // TODO(xushaohua): Filters EFI file system.
}

}  // namespace ui