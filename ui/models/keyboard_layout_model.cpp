// Copyright (c) 2016 Deepin Ltd. All rights reserved.
// Use of this source is governed by General Public License that can be found
// in the LICENSE file.

#include "ui/models/keyboard_layout_model.h"

#include <algorithm>

namespace installer {

KeyboardLayoutModel::KeyboardLayoutModel(QObject* parent)
    : QAbstractListModel(parent),
      xkb_config_(GetXkbConfig()) {
  this->setObjectName("keyboard_layout_model");

  layout_list_ = xkb_config_.layout_list;
  std::sort(layout_list_.begin(), layout_list_.end());
}

QVariant KeyboardLayoutModel::data(const QModelIndex& index, int role) const {
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (!index.isValid()) {
    return QVariant();
  }

  const int row = index.row();
  if (row >= layout_list_.length()) {
    return QVariant();
  }

  return layout_list_.at(row).description;
}

int KeyboardLayoutModel::rowCount(const QModelIndex& parent) const {
  Q_UNUSED(parent);
  return layout_list_.length();
}

}  // namespace installer