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