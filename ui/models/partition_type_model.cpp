// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/models/partition_type_model.h"

namespace installer {

namespace {

const char kTextLogical[] = "Logical Partition";
const char kTextPrimary[] = "Primary Partition";

}  // namespace

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
      return tr(kTextLogical);
    }
  } else if (row == 0) {
    if (is_primary_visible_) {
      return tr(kTextPrimary);
    } else if (is_logical_visible_) {
      return tr(kTextLogical);
    }
  }
  return QVariant();
}

int PartitionTypeModel::rowCount(const QModelIndex& parent) const {
  int rows = 0;
  if (is_primary_visible_) {
    rows ++;
  }
  if (is_logical_visible_) {
    rows ++;
  }
  return rows;
}

bool PartitionTypeModel::isLogical(const QModelIndex& index) const {
  // First make sure that logical partition is visible.
  if (is_logical_visible_) {
    const int row = index.row();
    if (is_primary_visible_) {
      return row == 1;
    } else {
      return row == 0;
    }
  }
  return false;
}

bool PartitionTypeModel::isPrimary(const QModelIndex& index) const {
  // First make sure that primary partition is visible.
  if (is_primary_visible_) {
    const int row = index.row();
    return row == 0;
  }
  return false;
}

void PartitionTypeModel::reset() {
  this->beginResetModel();
  is_logical_visible_ = true;
  is_primary_visible_ = true;
  this->endResetModel();
}

void PartitionTypeModel::setLogicalVisibel(bool visible) {
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