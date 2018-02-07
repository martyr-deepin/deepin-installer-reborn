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

#include "ui/models/partition_type_model.h"

namespace installer {

PartitionTypeModel::PartitionTypeModel(QObject* parent)
    : QAbstractListModel(parent),
      is_logical_visible_(true),
      is_primary_visible_(true) {
  this->setObjectName("partition_type_model");
}

QVariant PartitionTypeModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (!index.isValid()) {
    return QVariant();
  }

  const int row = index.row();
  if (row == 1) {
    if (is_primary_visible_ && is_logical_visible_) {
      return tr("Logical Partition");
    }
  } else if (row == 0) {
    if (is_primary_visible_) {
      return tr("Primary Partition");
    } else if (is_logical_visible_) {
      return tr("Logical Partition");
    }
  }
  return QVariant();
}

int PartitionTypeModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  int rows = 0;
  if (is_primary_visible_) {
    rows ++;
  }
  if (is_logical_visible_) {
    rows ++;
  }
  return rows;
}

int PartitionTypeModel::getLogicalIndex() const {
  // Check whether logical partition is visible.
  if (is_logical_visible_) {
    return is_primary_visible_ ? 1 : 0;
  }
  return -1;
}

int PartitionTypeModel::getPrimaryIndex() const {
  // Check whether primary partition is visible.
  return is_primary_visible_ ? 0 : -1;
}

bool PartitionTypeModel::isLogical(int index) const {
  // First make sure that logical partition is visible.
  if (is_logical_visible_) {
    return is_primary_visible_ ? (index == 1) : (index == 0);
  }
  return false;
}

bool PartitionTypeModel::isPrimary(int index) const {
  // First make sure that primary partition is visible.
  return is_primary_visible_ ? (index == 0) : false;
}

void PartitionTypeModel::reset() {
  this->beginResetModel();
  is_logical_visible_ = true;
  is_primary_visible_ = true;
  this->endResetModel();
}

void PartitionTypeModel::setLogicalVisible(bool visible) {
  this->beginResetModel();
  is_logical_visible_ = visible;
  this->endResetModel();
}

void PartitionTypeModel::setPrimaryVisible(bool visible) {
  this->beginResetModel();
  is_primary_visible_ = visible;
  this->endResetModel();
}

}  // namespace installer