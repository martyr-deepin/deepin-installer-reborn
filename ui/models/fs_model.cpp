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