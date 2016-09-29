// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "fs_model.h"

#include "ui/delegates/partition_delegate.h"
#include "ui/delegates/partition_util.h"

namespace installer {

FsModel::FsModel(PartitionDelegate* delegate, QObject* parent)
    : QAbstractListModel(parent),
      delegate_(delegate),
      fs_list_(delegate_->getFsTypes()) {
  this->setObjectName(QStringLiteral("fs_model"));
}

int FsModel::index(FsType fs_type) const {
  return fs_list_.indexOf(fs_type);
}

FsType FsModel::getFs(int index) const {
  Q_ASSERT(index < fs_list_.length());
  if (index < fs_list_.length()) {
    return fs_list_.at(index);
  } else {
    return FsType::Unknown;
  }
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

  const FsType fs = fs_list_.at(index.row());
  return GetLocalFsTypeName(fs);
}

}  // namespace installer