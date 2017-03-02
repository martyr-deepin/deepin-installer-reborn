// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "fs_model.h"

#include "ui/delegates/partition_util.h"

namespace installer {

FsModel::FsModel(const FsTypeList& fs_list, QObject* parent)
    : QAbstractListModel(parent),
      fs_list_(fs_list) {
  this->setObjectName("fs_model");
}

QVariant FsModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  const int row = index.row();
  if (index.isValid() && row >= 0 && row < fs_list_.length()) {
    const FsType fs = fs_list_.at(index.row());
    return GetLocalFsTypeName(fs);
  } else {
    return QVariant();
  }
}

int FsModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return fs_list_.length();
}

FsType FsModel::getFs(int index) const {
  Q_ASSERT(index < fs_list_.length());
  if (index >= 0 && index < fs_list_.length()) {
    return fs_list_.at(index);
  } else {
    return FsType::Unknown;
  }
}

int FsModel::index(FsType fs_type) const {
  // Might return -1.
  return fs_list_.indexOf(fs_type);
}

void FsModel::setShowEFI(bool show_efi) {
  this->beginResetModel();

  if (show_efi) {
    if (fs_list_.indexOf(FsType::EFI) == -1) {
      fs_list_.append(FsType::EFI);
    }
  } else {
    const int index = fs_list_.indexOf(FsType::EFI);
    if (index != -1) {
      fs_list_.removeAt(index);
    }
  }

  this->endResetModel();
}

void FsModel::setShowUnknown(bool show_unknown) {
  this->beginResetModel();

  if (show_unknown) {
    if (fs_list_.indexOf(FsType::Unknown) == -1) {
      fs_list_.append(FsType::Unknown);
    }
  } else {
    const int index = fs_list_.indexOf(FsType::Unknown);
    if (index != -1) {
      fs_list_.removeAt(index);
    }
  }

  this->endResetModel();
}

}  // namespace installer